/**
 * Copyright (c) 2023-present, Zejun Wang.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef LCS_H
#define LCS_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#if __cplusplus >= 201402L
#include "flat_hash_map/bytell_hash_map.hpp"
#else
#include "flat_hash_map/unordered_map.hpp"
#endif

using namespace std;
using byte_t = uint_fast8_t;
using code_t = char16_t;

namespace fastlcs {

struct Tuple {
  uint32_t b1;
  uint32_t b2;
  uint32_t len;
};

static inline void err(const char* fn, const int ln, const char* msg) {
  cerr << fn << " [" << ln << "]: " << msg;
  exit(1);
}

static inline void set_result(Tuple* t, uint32_t b1, uint32_t b2, uint32_t len) noexcept {
  t->b1 = b1;
  t->b2 = b2;
  t->len = len;
}

inline byte_t get_num_bytes_of_utf8_char(const char* str, size_t len) noexcept {
  size_t cur = 1;
  byte_t num_bytes = 1;
  while (cur < len && (str[cur++] & 0xC0) == 0x80)
    ++num_bytes;
  return num_bytes;
}

inline size_t get_num_codepoints(const char* str, size_t len) noexcept {
  size_t cur = 0, num = 0;
  while (cur < len) {
    cur += get_num_bytes_of_utf8_char(str + cur, len - cur);
    ++num;
  }
  return num;
}

template <typename T>
inline byte_t char_unicode(const char* str, size_t len, T& cp) noexcept {
  byte_t num_bytes = get_num_bytes_of_utf8_char(str, len);
  cp = (unsigned char)*str;
  if (num_bytes > 1) {
    cp &= 0x7F >> num_bytes;
    for (byte_t i = 1; i < num_bytes; ++i)
      cp = (cp << 6) | ((unsigned char)str[i] & 0x3F);    
  }
  return num_bytes;
}

template <typename T>
inline size_t unicode(const char* str, size_t len, T* data) noexcept {
  T cp;
  byte_t num_bytes;
  size_t cur = 0, num = 0;
  while (cur < len) {
    num_bytes = char_unicode <T> (str + cur, len - cur, cp);
    data[num] = cp;
    ++num;
    cur += num_bytes;
  }
  return num;
}

inline uint64_t hashstr(const char* str) noexcept {
  uint64_t hash = 5381;
  uint8_t c;
  --str;
  while (c = *++str)
    hash = ((hash << 5) + hash) + c;
  return hash;
}

// Dynamic programming for length of LCS
// Time complexity O(mn)
// Space complexity O(min(m,n))
template <typename T>
uint32_t lcs_len_dp_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2) {
  if (len1 < len2)
    return lcs_len_dp_impl <T> (data2, len2, data1, len1);
  if (len2 == 0)
    return 0;
  // trim off the matching items at the beginning
  uint32_t prefix = 0, suffix = 0;
  while (len2 > 0 && *data1 == *data2) {
    ++prefix;
    ++data1;
    ++data2;
    --len2;
    --len1;
  }
  // trim off the matching items at the end
  while (len2 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    ++suffix;
    --len1;
    --len2;
  }
  if (len2 == 0)
    return prefix + suffix;
  // dynamic programming
  uint32_t temp, bottom_right;
  uint32_t* dp = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
  if (!dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  memset(dp, 0, sizeof(uint32_t) * (len2 + 1));
  for (int64_t i = len1 - 1; i >= 0; --i) {
    bottom_right = 0;
    for (int64_t j = len2 - 1; j >= 0; --j) {
      temp = dp[j];
      if (data1[i] == data2[j])
        dp[j] = bottom_right + 1;
      else
        dp[j] = max(dp[j], dp[j + 1]);
      bottom_right = temp;
    }
  }
  temp = *dp + prefix + suffix;
  free(dp);
  return temp;
}

// Longest Increasing Subsequence
// Faster than dynamic programming on average
template <typename T>
uint32_t lcs_len_map_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2) {
  if (len1 < len2)
    return lcs_len_map_impl <T> (data2, len2, data1, len1);
  if (len2 == 0)
    return 0;
  // trim off the matching items at the beginning
  uint32_t prefix = 0, suffix = 0;
  while (len2 > 0 && *data1 == *data2) {
    ++prefix;
    ++data1;
    ++data2;
    --len2;
    --len1;
  }
  // trim off the matching items at the end
  while (len2 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    ++suffix;
    --len1;
    --len2;
  }
  if (len2 == 0)
    return prefix + suffix;
#if __cplusplus >= 201402L
  ska::bytell_hash_map<T, vector<uint32_t>> ska_map;
#else
  ska::unordered_map<T, vector<uint32_t>> ska_map;
#endif
  T key;
  for (uint32_t i = 0; i < len2; ++i) {
    key = data2[i];
    if (ska_map.count(key))
      ska_map[key].emplace_back(i);
    else {
#ifdef LIS_RESERVE
      vector<uint32_t> val;
      val.reserve(len2 >> 2 + 1);
      val.emplace_back(i);
      ska_map[key] = move(val);
#else
      ska_map[key] = {i};
#endif
    }
  }
  // Longest Increasing Subsequence
  uint32_t index, cur = 0;
  uint32_t* a = (uint32_t*) malloc(sizeof(uint32_t) * len2);
  if (!a)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (uint32_t i = 0; i < len1; ++i) {
    key = data1[i];
    if (!ska_map.count(key))
      continue;
    vector<uint32_t>::const_reverse_iterator iter, end(ska_map[key].rend());
    for (iter = ska_map[key].rbegin(); iter != end; ++iter) {
      index = *iter;
      if (cur == 0 || index > a[cur - 1])
        a[cur++] = index;
      else
        *lower_bound(a, a + cur, index) = index;
    }
  }
  free(a);
  return cur + prefix + suffix;
}

// Dynamic programming for LCS with subsequence position
// Time complexity O(mn)
// Space complexity O(mn)
template <typename T>
Tuple* lcs_dp_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2, uint32_t& size) {
  if (len1 < len2)
    return lcs_dp_impl <T> (data2, len2, data1, len1, size);
  if (len2 == 0)
    return NULL;
  // trim off the matching items at the beginning
  uint32_t prefix = 0, suffix = 0;
  while (len2 > 0 && *data1 == *data2) {
    ++prefix;
    ++data1;
    ++data2;
    --len2;
    --len1;
  }
  // trim off the matching items at the end
  while (len2 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    ++suffix;
    --len1;
    --len2;
  }
  if (len2 == 0) {
    Tuple* result = (Tuple*) malloc(sizeof(Tuple) * 2);
    if (!result)
      err(__FILE__, __LINE__, "memory reallocation failed\n");
    size = 0;
    if (prefix > 0)
      set_result(result + size++, 0, 0, prefix);
    if (suffix > 0)
      set_result(result + size++, prefix + len1, prefix, suffix);
    return result;
  }
  // dynamic programming
  uint32_t** dp = (uint32_t**) malloc(sizeof(uint32_t*) * (len1 + 1));
  if (!dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (uint32_t i = 0; i <= len1; ++i) {
    dp[i] = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
    if (!dp[i])
      err(__FILE__, __LINE__, "memory reallocation failed\n");
  }
  for (uint32_t i = 0; i <= len1; ++i)
    dp[i][len2] = 0;
  for (uint32_t j = 0; j <= len2; ++j)
    dp[len1][j] = 0;
  for (int64_t i = len1 - 1; i >= 0; --i) {
    for (int64_t j = len2 - 1; j >= 0; --j) {
      if (data1[i] == data2[j])
        dp[i][j] = dp[i + 1][j + 1] + 1;
      else
        dp[i][j] = max(dp[i][j + 1], dp[i + 1][j]);
    }
  }
  // subsequence position
  size = 0;
  uint32_t len = **dp;
  if (len == 0) {
    Tuple* result = (Tuple*) malloc(sizeof(Tuple) * 2);
    if (!result)
      err(__FILE__, __LINE__, "memory reallocation failed\n");
    if (prefix > 0)
      set_result(result + size++, 0, 0, prefix);
    if (suffix > 0)
      set_result(result + size++, prefix + len1, prefix + len2, suffix);
    // deallocate memory
    for (uint32_t i = 0; i <= len1; ++i)
      free(dp[i]);
    free(dp);
    return result;
  }
  uint32_t x = 0, y = 0, n = 0;
  uint32_t* equal = (uint32_t*) malloc(sizeof(uint32_t) * (len << 1));
  if (!equal)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  while (x < len1 && y < len2) {
    if (data1[x] == data2[y]) {
      equal[n++] = x + prefix;
      equal[n++] = y + prefix;
      ++x;
      ++y;
    } else if (dp[x][y] == dp[x][y + 1])
      ++y;
    else
      ++x;
  }
  Tuple* result = (Tuple*) malloc(sizeof(Tuple) * (len + 2));
  if (!result)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  if (prefix > 0)
    set_result(result + size++, 0, 0, prefix);
  uint32_t b1 = equal[0], b2 = equal[1];
  uint32_t e1 = b1 + 1, e2 = b2 + 1;
  for (uint32_t i = 2; i < n; i += 2) {
    if (e1 == equal[i] && e2 == equal[i + 1]) {
      ++e1;
      ++e2;
      continue;
    }
    set_result(result + size++, b1, b2, e2 - b2);
    b1 = equal[i];
    b2 = equal[i + 1];
    e1 = b1 + 1;
    e2 = b2 + 1;
  }
  set_result(result + size++, b1, b2, e2 - b2);
  if (suffix > 0)
    set_result(result + size++, prefix + len1, prefix + len2, suffix);
  // deallocate memory
  for (uint32_t i = 0; i <= len1; ++i)
    free(dp[i]);
  free(dp);
  free(equal);
  return result;
}

// Hirschberg's algorithm for LCS
// Time complexity O(mn)
// Space complexity O(min(m,n))
template <typename T>
void lcs_dp_left(const T* a, uint32_t a_len, const T* b, uint32_t b_len, uint32_t* dp) {
  uint32_t temp, top_left;
  for (uint32_t i = 1; i <= a_len; ++i) {
    top_left = 0;
    for (uint32_t j = 1; j <= b_len; ++j) {
      temp = dp[j];
      if (a[i - 1] == b[j - 1])
        dp[j] = top_left + 1;
      else
        dp[j] = max(dp[j], dp[j - 1]);
      top_left = temp;
    }
  }
}

template <typename T>
void lcs_dp_right(const T* a, uint32_t a_len, const T* b, uint32_t b_len, uint32_t* dp) {
  uint32_t temp, bottom_right;
  for (int64_t i = a_len - 1; i >= 0; --i) {
    bottom_right = 0;
    for (int64_t j = b_len - 1; j >= 0; --j) {
      temp = dp[j];
      if (a[i] == b[j])
        dp[j] = bottom_right + 1;
      else
        dp[j] = max(dp[j], dp[j + 1]);
      bottom_right = temp;
    }
  }
}

template <typename T>
void lcs_hirschberg_recursive(const T* a, uint32_t a_start, uint32_t a_len, const T* b, uint32_t b_start, 
    uint32_t b_len, uint32_t* dp_left, uint32_t* dp_right, uint32_t* equal, uint32_t& n) {
  if (b_len == 0)
    return;
  if (a_len == 1) {
    uint32_t pos = find(b + b_start, b + b_start + b_len, a[a_start]) - b;
    if (pos < b_start + b_len) {
      equal[n++] = a_start;
      equal[n++] = pos;
    }
    return;
  }
  uint32_t mid = a_len / 2;
  lcs_dp_left <T> (a + a_start, mid, b + b_start, b_len, dp_left);
  lcs_dp_right <T> (a + a_start + mid, a_len - mid, b + b_start, b_len, dp_right);
  uint32_t k = 0, sum = 0, temp = 0;
  for (uint32_t j = 0; j <= b_len; ++j) {
    sum = dp_left[j] + dp_right[j];
    if (sum > temp) {
      temp = sum;
      k = j;
    }
    dp_left[j] = 0;
    dp_right[j] = 0;
  }
  lcs_hirschberg_recursive <T> (a, a_start, mid, b, b_start, k, dp_left, dp_right, equal, n);
  lcs_hirschberg_recursive <T> (
    a, a_start + mid, a_len - mid, b, b_start + k, b_len - k, dp_left, dp_right, equal, n
  );
}

template <typename T>
Tuple* lcs_hirschberg_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2, uint32_t& size) {
  if (len1 < len2)
    return lcs_hirschberg_impl <T> (data2, len2, data1, len1, size);
  if (len2 == 0)
    return NULL;
  // trim off the matching items at the beginning
  uint32_t prefix = 0, suffix = 0;
  while (len2 > 0 && *data1 == *data2) {
    ++prefix;
    ++data1;
    ++data2;
    --len2;
    --len1;
  }
  // trim off the matching items at the end
  while (len2 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    ++suffix;
    --len1;
    --len2;
  }
  if (len2 == 0) {
    Tuple* result = (Tuple*) malloc(sizeof(Tuple) * 2);
    if (!result)
      err(__FILE__, __LINE__, "memory reallocation failed\n");
    size = 0;
    if (prefix > 0)
      set_result(result + size++, 0, 0, prefix);
    if (suffix > 0)
      set_result(result + size++, prefix + len1, prefix, suffix);
    return result;
  }
  uint32_t n = 0;
  uint32_t* equal = (uint32_t*) malloc(sizeof(uint32_t) * (len2 << 1));
  uint32_t* dp_left = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
  uint32_t* dp_right = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
  if (!equal || !dp_left || !dp_right)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (uint32_t i = 0; i <= len2; i++) {
    dp_left[i] = 0;
    dp_right[i] = 0;
  }
  lcs_hirschberg_recursive <T> (data1, 0, len1, data2, 0, len2, dp_left, dp_right, equal, n);
  size = 0;
  if (n == 0) {
    Tuple* result = (Tuple*) malloc(sizeof(Tuple) * 2);
    if (!result)
      err(__FILE__, __LINE__, "memory reallocation failed\n");
    if (prefix > 0)
      set_result(result + size++, 0, 0, prefix);
    if (suffix > 0)
      set_result(result + size++, prefix + len1, prefix + len2, suffix);
    // deallocate memory
    free(equal);
    free(dp_left);
    free(dp_right);
    return result;
  }
  Tuple* result = (Tuple*) malloc(sizeof(Tuple) * ((n >> 1) + 2));
  if (!result)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  if (prefix > 0)
    set_result(result + size++, 0, 0, prefix);
  uint32_t b1 = equal[0], b2 = equal[1];
  uint32_t e1 = b1 + 1, e2 = b2 + 1;
  for (uint32_t i = 2; i < n; i += 2) {
    if (e1 == equal[i] && e2 == equal[i + 1]) {
      ++e1;
      ++e2;
      continue;
    }
    set_result(result + size++, b1 + prefix, b2 + prefix, e2 - b2);
    b1 = equal[i];
    b2 = equal[i + 1];
    e1 = b1 + 1;
    e2 = b2 + 1;
  }
  set_result(result + size++, b1 + prefix, b2 + prefix, e2 - b2);
  if (suffix > 0)
    set_result(result + size++, prefix + len1, prefix + len2, suffix);
  // deallocate memory
  free(equal);
  free(dp_left);
  free(dp_right);
  return result;
}

// Dynamic programming for longest common substring
// Time complexity O(mn)
// Space complexity O(min(m,n))
template <typename T>
Tuple lcsubstr_dp_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2) {
  if (len1 < len2)
    return lcsubstr_dp_impl <T> (data2, len2, data1, len1);
  Tuple result = {0, 0, 0};
  if (len2 == 0)
    return result;
  uint32_t b1 = 0, b2 = 0, len = 0;
  uint32_t* dp = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
  if (!dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  memset(dp, 0, sizeof(uint32_t) * (len2 + 1));
  for (int64_t i = len1 - 1; i >= 0; --i) {
    for (uint32_t j = 0; j < len2; ++j) {
      if (data1[i] == data2[j]) {
        dp[j] = dp[j + 1] + 1;
        if (dp[j] > len) {
          len = dp[j];
          b1 = i;
          b2 = j;
        }
      } else
        dp[j] = 0;
    }
  }
  set_result(&result, b1, b2, len);
  free(dp);
  return result;
}

template <typename T>
Tuple lcsubstr_diag_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2) {
  if (len1 < len2)
    return lcsubstr_diag_impl <T> (data2, len2, data1, len1);
  Tuple result = {0, 0, 0};
  if (len2 == 0)
    return result;
  uint32_t m, n, e1 = 0, e2 = 0, len = 0, longest = 0;
  for (uint32_t i = 0; i < len1; ++i) {
    // reduce some unnecessary comparisons
    if (len2 <= longest || len1 <= i + longest)
      break;
    m = i;
    n = 0;
    len = 0;
    while (m < len1 && n < len2) {
      if (data1[m] == data2[n]) {
        ++len;
        if (longest < len) {
          longest = len;
          e1 = m;
          e2 = n;
        }
      } else
        len = 0;
      ++m;
      ++n;
    }
  }
  for (uint32_t j = 1; j < len2; ++j) {
    // reduce some unnecessary comparisons
    if (len2 <= j + longest)
      break;
    m = 0;
    n = j;
    len = 0;
    while (n < len2) {
      if (data1[m] == data2[n]) {
        ++len;
        if (longest < len) {
          longest = len;
          e1 = m;
          e2 = n;
        }
      } else
        len = 0;
      ++m;
      ++n;
    }
  }
  set_result(&result, e1 + 1 - longest, e2 + 1 - longest, longest);
  return result;
}

// Dynamic programming for Levenshtein distance
// Time complexity O(mn)
// Space complexity O(min(m,n))
template <typename T>
uint32_t edit_distance_impl(const T* data1, uint32_t len1, const T* data2, uint32_t len2) {
  if (len1 < len2)
    return edit_distance_impl <T> (data2, len2, data1, len1);
  if (len2 == 0)
    return len1;
  // trim off the matching items at the beginning
  uint32_t prefix = 0, suffix = 0;
  while (len2 > 0 && *data1 == *data2) {
    ++prefix;
    ++data1;
    ++data2;
    --len2;
    --len1;
  }
  // trim off the matching items at the end
  while (len2 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    ++suffix;
    --len1;
    --len2;
  }
  if (len2 == 0)
    return len1;
  uint32_t cost, temp, top_left;
  uint32_t* dp = (uint32_t*) malloc(sizeof(uint32_t) * (len2 + 1));
  if (!dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (uint32_t i = 0; i <= len2; ++i)
    dp[i] = i;
  for (uint32_t i = 1; i <= len1; ++i) {
    *dp = i;
    top_left = i - 1;
    for (uint32_t j = 1; j <= len2; ++j) {
      temp = dp[j];
      cost = (data1[i - 1] == data2[j - 1]) ? 0 : 1;
      dp[j] = min(min(dp[j], dp[j - 1]) + 1, top_left + cost);
      top_left = temp;
    }
  }
  temp = dp[len2];
  free(dp);
  return temp;
}

// Implementation of bounded Levenshtein distance (Ukkonen)
// Time Complexity O(min(m,n)*k)
// Space Complexity O(k)
template <typename T>
int64_t edit_distance_k_impl(const T* data1, int64_t len1, const T* data2, int64_t len2, int64_t k) {
  if (len1 > len2)
    return edit_distance_k_impl(data2, len2, data1, len1, k);
  // trim off the matching items at the beginning
  while (len1 > 0 && *data1 == *data2) {
    ++data1;
    ++data2;
    --len1;
    --len2;
  }
  // trim off the matching items at the end
  while (len1 > 0 && data1[len1 - 1] == data2[len2 - 1]) {
    --len1;
    --len2;
  }
  k = min(k, len2);
  if (len1 == 0)
    return k;
  int64_t d_len = len2 - len1;
  if (d_len > k)
    return k;
  int64_t ZERO_K = min(k, len1) / 2 + 2;
  int64_t array_len = d_len + ZERO_K * 2 + 2;
  int64_t* current_row = (int64_t*) malloc(sizeof(int64_t) * array_len);
  int64_t* next_row = (int64_t*) malloc(sizeof(int64_t) * array_len);
  if (!current_row || !next_row)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (uint32_t i = 0; i < array_len; i++) {
    current_row[i] = -1;
    next_row[i] = -1;
  }
  int64_t i = 0, condition_row = d_len + ZERO_K; 
  int64_t end_max = condition_row * 2;
  int64_t start, end, previous_cell, current_cell, next_cell;
  do {
    ++i;
    swap(current_row, next_row);
    current_cell = -1;
    if (i <= ZERO_K) {
      start = -i + 1;
      next_cell = i - 2;
    } else {
      start = i - ZERO_K * 2 + 1;
      next_cell = current_row[ZERO_K + start];
    }
    if (i <= condition_row) {
      end = i;
      next_row[ZERO_K + i] = -1;
    } else
      end = end_max - i;
    for (int64_t j = start, row_index = start + ZERO_K; j < end; ++j, ++row_index) {
      previous_cell = current_cell;
      current_cell = next_cell;
      next_cell = current_row[row_index + 1];
      int64_t t = max(max(previous_cell, current_cell + 1), next_cell + 1);
      while (t < len1 && t + j < len2 && data1[t] == data2[t + j])
        ++t;
      next_row[row_index] = t;
    }
  } while (next_row[condition_row] < len1 && i <= k);
  free(current_row);
  free(next_row);
  return i - 1;
}

inline uint32_t lcs_len_dp(const string& s1, const string& s2) {
  if (s1.empty() || s2.empty())
    return 0;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  uint32_t result = lcs_len_dp_impl <code_t> (data1, len1, data2, len2);
  free(data1);
  free(data2);
  return result;
}

inline uint32_t lcs_len_map(const string& s1, const string& s2) {
  if (s1.empty() || s2.empty())
    return 0;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  uint32_t result = lcs_len_map_impl <code_t> (data1, len1, data2, len2);
  free(data1);
  free(data2);
  return result;
}

inline Tuple* lcs_dp(const string& s1, const string& s2, uint32_t& size) {
  if (s1.empty() || s2.empty())
    return NULL;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  Tuple* result = lcs_dp_impl <code_t> (data1, len1, data2, len2, size);
  if (len1 < len2) {
    for (uint32_t i = 0; i < size; i++)
      swap(result[i].b1, result[i].b2);
  }
  free(data1);
  free(data2);
  return result;
}

inline Tuple* lcs_hirschberg(const string& s1, const string& s2, uint32_t& size) {
  if (s1.empty() || s2.empty())
    return NULL;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  Tuple* result = lcs_hirschberg_impl <code_t> (data1, len1, data2, len2, size);
  if (len1 < len2) {
    for (uint32_t i = 0; i < size; i++)
      swap(result[i].b1, result[i].b2);
  }
  free(data1);
  free(data2);
  return result;
}

inline Tuple lcsubstr_dp(const string& s1, const string& s2) {
  Tuple result = {0, 0, 0};
  if (s1.empty() || s2.empty())
    return result;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  result = lcsubstr_dp_impl <code_t> (data1, len1, data2, len2);
  if (len1 < len2)
    swap(result.b1, result.b2);
  free(data1);
  free(data2);
  return result;
}

inline Tuple lcsubstr_diag(const string& s1, const string& s2) {
  Tuple result = {0, 0, 0};
  if (s1.empty() || s2.empty())
    return result;
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  result = lcsubstr_diag_impl <code_t> (data1, len1, data2, len2);
  if (len1 < len2)
    swap(result.b1, result.b2);
  free(data1);
  free(data2);
  return result; 
}

inline uint32_t edit_distance(const string& s1, const string& s2) {
  if (s1.empty())
    return get_num_codepoints(s2.data(), s2.size());
  if (s2.empty())
    return get_num_codepoints(s1.data(), s1.size());
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  uint32_t distance = edit_distance_impl <code_t> (data1, len1, data2, len2);
  free(data1);
  free(data2);
  return distance; 
}

inline uint32_t edit_distance_k(const string& s1, const string& s2, uint32_t k) {
  if (s1.empty())
    return get_num_codepoints(s2.data(), s2.size());
  if (s2.empty())
    return get_num_codepoints(s1.data(), s1.size());
  code_t* data1 = (code_t*) malloc(sizeof(code_t) * s1.size());
  code_t* data2 = (code_t*) malloc(sizeof(code_t) * s2.size());
  if (!data1 || !data2)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  uint32_t len1 = unicode <code_t> (s1.data(), s1.size(), data1);
  uint32_t len2 = unicode <code_t> (s2.data(), s2.size(), data2);
  uint32_t distance = edit_distance_k_impl <code_t> (data1, len1, data2, len2, k);
  free(data1);
  free(data2);
  return distance;
}

}
#endif

