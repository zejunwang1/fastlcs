/**
 * Copyright (c) 2023-present, Zejun Wang.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef LCS_H
#define LCS_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <tuple>
#include <vector>
#include "flat_hash_map/unordered_map.hpp"

namespace lcs {

typedef unsigned char byte_t;
typedef unsigned int  code_t;

using seq = std::tuple<code_t, code_t, code_t>;

code_t strlcs_len(const char* a, code_t alen, const char* b, code_t blen);

code_t strlcs_len(const std::string& a, const std::string& b);

code_t strlcs_len_map(const char* a, code_t alen, const char* b, code_t blen);

code_t strlcs_len_map(const std::string& a, const std::string& b);

std::vector<seq> strlcs_dp(const char* a, code_t alen, const char* b, code_t blen);

std::vector<seq> strlcs_dp(const std::string& a, const std::string& b);

std::vector<seq> strlcs_hirschberg(const char* a, code_t alen, const char* b, code_t blen);

std::vector<seq> strlcs_hirschberg(const std::string& a, const std::string& b);

std::vector<seq> strlcs_myers(const char* a, code_t alen, const char* b, code_t blen);

std::vector<seq> strlcs_myers(const std::string& a, const std::string& b);

seq longest_match(const char* a, code_t alen, const char* b, code_t blen);

seq longest_match(const std::string& a, const std::string& b);

seq longest_match_diag(const char* a, code_t alen, const char* b, code_t blen);

seq longest_match_diag(const std::string& a, const std::string& b);

code_t str_edit_distance(const char* a, code_t alen, const char* b, code_t blen);

code_t str_edit_distance(const std::string& a, const std::string& b);

code_t str_edit_distance_k(const char* a, code_t alen, const char* b, code_t blen, code_t k);

code_t str_edit_distance_k(const std::string& a, const std::string& b, code_t k);

template <typename T>
void snake(
    const T* a, code_t a_start, code_t a_end,
    const T* b, code_t b_start, code_t b_end,
    int* down, int* up, std::vector<seq>& result
);

template <typename T>
void shortest_edit_script(
    const T* a, code_t a_start, code_t a_end,
    const T* b, code_t b_start, code_t b_end,
    int* down, int* up, std::vector<seq>& result
);

template <typename T>
code_t lcs_len(const T* a, code_t n, const T* b, code_t m);

template <typename T>
code_t lcs_len_map(const T* a, code_t n, const T* b, code_t m);

template <typename T>
std::vector<seq> lcs_dp(const T* a, code_t n, const T* b, code_t m);

template <typename T>
std::vector<seq> lcs_hirschberg(const T* a, code_t n, const T* b, code_t m);

template <typename T>
std::vector<seq> lcs_myers(const T* a, code_t n, const T* b, code_t m);

template <typename T>
seq find_longest_match(const T* a, code_t n, const T* b, code_t m);

template <typename T>
seq find_longest_match_diag(const T* a, code_t n, const T* b, code_t m);

template <typename T>
code_t edit_distance(const T* a, code_t n, const T* b, code_t m);

template <typename T>
int edit_distance_k(const T* a, int n, const T* b, int m, int k);

inline void err(const char* fn, const int ln, const char* msg) {
  fprintf(stderr, "%s [%d]: %s", fn, ln, msg);
  exit(1);
}

inline byte_t get_num_bytes_of_utf8_char(char first_byte) noexcept {
  //assert((first_byte & 0xC0) != 0x80);
  if ((first_byte & 0x80) == 0)
    return 1;
  if ((first_byte & 0xE0) == 0xC0)
    return 2;
  if ((first_byte & 0xF0) == 0xE0)
    return 3;
  return 4;
}

inline byte_t decode_utf8(const char* str, code_t& code) noexcept {
  byte_t num_bytes = get_num_bytes_of_utf8_char(*str);
  code = (unsigned char)*str;
  if (num_bytes > 1) {
    code &= 0x7F >> num_bytes;
    for (byte_t i = 1; i < num_bytes; ++i)
      code = (code << 6) | ((unsigned char)str[i] & 0x3F);
  }
  return num_bytes;
}

inline code_t decode(const char* str, code_t len, code_t* codes) {
  byte_t num_bytes;
  code_t code, cur = 0, n = 0;
  while (cur < len) {
    num_bytes = decode_utf8(str + cur, code);
    codes[n++] = code;
    cur += num_bytes;
  }
  return n;
}

template <typename T>
code_t lcs_len(const T* a, code_t n, const T* b, code_t m) {
  if (n < m)
    return lcs_len<T>(b, m, a, n);
  if (m == 0)
    return 0;
  // trim off the matching items at the beginning
  code_t prefix = 0, suffix = 0;
  while (m > 0 && *a == *b) {
    ++prefix;
    ++a;
    ++b;
    --n;
    --m;
  }
  // trim off the matching items at the end
  while (m > 0 && a[n - 1] == b[m - 1]) {
    ++suffix;
    --n;
    --m;
  }
  if (m == 0)
    return prefix + suffix;
  // Dynamic Programming
  code_t temp, bottom_right;
  code_t* dp = (code_t*) malloc((m + 1) * sizeof(code_t));
  if (! dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  memset(dp, 0, sizeof(code_t) * (m + 1));
  for (int i = n - 1; i >= 0; --i) {
    bottom_right = 0;
    for (int j = m - 1; j >= 0; --j) {
      temp = dp[j];
      if (a[i] == b[j])
        dp[j] = bottom_right + 1;
      else
        dp[j] = std::max(dp[j], dp[j + 1]);
      bottom_right = temp;
    }
  }
  temp = *dp + prefix + suffix;
  free(dp); dp = 0;
  return temp;
}

template <typename T>
code_t lcs_len_map(const T* a, code_t n, const T* b, code_t m) {
  if (n < m)
    return lcs_len_map<T>(b, m, a, n);
  if (m == 0)
    return 0;
  code_t prefix = 0, suffix = 0;
  // trim off the matching items at the beginning
  while (m > 0 && *a == *b) {
    ++prefix;
    ++a;
    ++b;
    --n;
    --m;
  }
  // trim off the matching items at the end
  while (m > 0 && a[n - 1] == b[m - 1]) {
    ++suffix;
    --n;
    --m;
  }
  if (m == 0)
    return prefix + suffix;
  T key;
  ska::unordered_map<T, std::vector<code_t>> ska_map;
  for (code_t i = 0; i < m; ++i) {
    key = b[i];
    if (ska_map.count(key))
      ska_map[key].emplace_back(i);
    else
      ska_map[key] = { i };
  }
  // Longest Increasing Subsequence
  code_t index, k = 0;
  code_t* s = (code_t*) malloc(m * sizeof(code_t));
  if (! s)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (code_t i = 0; i < n; ++i) {
    key = a[i];
    if (! ska_map.count(key)) continue;
    std::vector<code_t>::const_reverse_iterator iter(ska_map[key].rbegin()), end(ska_map[key].rend());
    while (iter != end) {
      index = *iter;
      if (k == 0 || index > s[k - 1])
        s[k++] = index;
      else
        *std::lower_bound(s, s + k, index) = index;
      ++iter;
    }
  }
  free(s); s = 0;
  return prefix + suffix + k;
}

template <typename T>
std::vector<seq> lcs_dp(const T* a, code_t n, const T* b, code_t m) {
  std::vector<seq> result;
  code_t prefix = 0, suffix = 0;
  // trim off the matching items at the beginning
  while (n > 0 && m > 0 && *a == *b) {
    ++prefix;
    ++a;
    ++b;
    --n; 
    --m;
  }
  // trim off the matching items at the end
  while (n > 0 && m > 0 && a[n - 1] == b[m - 1]) {
    ++suffix;
    --n;
    --m;
  }
  if (prefix > 0)
    result.emplace_back(0, 0, prefix);
  if (n == 0 || m == 0) {
    if (suffix > 0)
      result.emplace_back(prefix + n, prefix + m, suffix);
    return result;
  }
  // Dynamic Programming
  code_t** dp = (code_t**) malloc((n + 1) * sizeof(code_t*));
  if (! dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (code_t i = 0; i <= n; ++i) {
    dp[i] = (code_t*) malloc((m + 1) * sizeof(code_t));
    if (! dp[i])
      err(__FILE__, __LINE__, "memory reallocation failed\n");
  }
  for (code_t i = 0; i <= n; ++i)
    dp[i][m] = 0;
  for (code_t j = 0; j <= m; ++j)
    dp[n][j] = 0;
  for (int i = n - 1; i >= 0; --i) {
    for (int j = m - 1; j >= 0; --j) {
      if (a[i] == b[j])
        dp[i][j] = dp[i + 1][j + 1] + 1;
      else
        dp[i][j] = std::max(dp[i][j + 1], dp[i + 1][j]);
    }
  }
  // Longest Common Subsequence
  code_t len = **dp;
  if (len == 0) {
    if (suffix > 0)
      result.emplace_back(prefix + n, prefix + m, suffix);
    // deallocate memory
    for (code_t i = 0; i <= n; ++i) free(dp[i]);
    free(dp); dp = 0;
    return result; 
  }
  result.reserve(len + 2);
  code_t i = 0, j = 0, x = n, y = m;
  len = 0;
  while (i < n && j < m) {
    if (a[i] == b[j]) {
      if (x == n) { x = i, y = j; }
      ++len;
      ++i;
      ++j;
    }
    else {
      if (x != n) {
        result.emplace_back(prefix + x, prefix + y, len);
        len = 0;
        x = n;
      }
      if (dp[i][j] == dp[i][j + 1]) ++j;
      else  ++i;
    }
  }
  if (len)
    result.emplace_back(prefix + x, prefix + y, len);
  if (suffix > 0)
    result.emplace_back(prefix + n, prefix + m, suffix);
  // deallocate memory
  for (code_t i = 0; i <= n; ++i) free(dp[i]);
  free(dp); dp = 0;
  return result;
}

template <typename T>
void hirschberg_left(const T* a, code_t n, const T* b, code_t m, code_t* dp) {
  code_t temp, top_left;
  for (code_t i = 1; i <= n; ++i) {
    top_left = 0;
    for (code_t j = 1; j <= m; ++j) {
      temp = dp[j];
      if (a[i - 1] == b[j - 1])
        dp[j] = top_left + 1;
      else
        dp[j] = std::max(dp[j], dp[j - 1]);
      top_left = temp;
    }
  }
}

template <typename T>
void hirschberg_right(const T* a, code_t n, const T* b, code_t m, code_t* dp) {
  code_t temp, bottom_right;
  for (int i = n - 1; i >= 0; --i) {
    bottom_right = 0;
    for (int j = m - 1; j >= 0; --j) {
      temp = dp[j];
      if (a[i] == b[j])
        dp[j] = bottom_right + 1;
      else
        dp[j] = std::max(dp[j], dp[j + 1]);
      bottom_right = temp;
    }
  }
}

template <typename T>
void hirschberg_recursive(
    const T* a, code_t a_start, code_t n, const T* b, code_t b_start, code_t m,
    code_t* left, code_t* right, code_t* match, code_t& num
) {
  if (m == 0) return;
  if (n == 1) {
    code_t p = std::find(b + b_start, b + b_start + m, a[a_start]) - b;
    if (p < b_start + m) {
      match[num++] = a_start;
      match[num++] = p;
    }
    return;
  }
  code_t middle = n / 2;
  hirschberg_left<T>(a + a_start, middle, b + b_start, m, left);
  hirschberg_right<T>(a + a_start + middle, n - middle, b + b_start, m, right);
  code_t split = 0, sum = 0, temp = 0;
  for (code_t j = 0; j <= m; ++j) {
    sum = left[j] + right[j];
    if (sum > temp) { temp = sum; split = j; }
    left[j] = 0;
    right[j] = 0;
  }
  hirschberg_recursive<T>(a, a_start, middle, b, b_start, split, left, right, match, num);
  hirschberg_recursive<T>(a, a_start + middle, n - middle, 
      b, b_start + split, m - split, left, right, match, num);
}

template <typename T>
std::vector<seq> lcs_hirschberg(const T* a, code_t n, const T* b, code_t m) {
  if (n < m) {
    std::vector<seq> result = std::move(lcs_hirschberg<T>(b, m, a, n));
    for (auto& t : result)
      std::swap(std::get<0>(t), std::get<1>(t));
    return result;
  }
  std::vector<seq> result;
  code_t prefix = 0, suffix = 0;
  // trim off the matching items at the beginning
  while (m > 0 && *a == *b) {
    ++prefix;
    ++a;
    ++b;
    --n;
    --m;
  }
  // trim off the matching items at the end
  while (m > 0 && a[n - 1] == b[m - 1]) {
    ++suffix;
    --n;
    --m;
  }
  if (prefix > 0)
    result.emplace_back(0, 0, prefix);
  if (m == 0) {
    if (suffix > 0)
      result.emplace_back(prefix + n, prefix + m, suffix);
    return result;
  }
  code_t num = 0;
  code_t* left  = (code_t*) malloc((m + 1) * sizeof(code_t));
  code_t* right = (code_t*) malloc((m + 1) * sizeof(code_t));
  code_t* match = (code_t*) malloc((m << 1) * sizeof(code_t));
  if (! left || ! right || ! match)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (code_t i = 0; i <= m; ++i) {
    left[i]   = 0;
    right[i]  = 0;
  }
  hirschberg_recursive<T>(a, 0, n, b, 0, m, left, right, match, num);
  if (num == 0) {
    if (suffix > 0)
      result.emplace_back(prefix + n, prefix + m, suffix);
    // deallocate memory
    free(left); left = 0;
    free(right);  right = 0;
    free(match);  match = 0;
    return result;
  }
  result.reserve((num >> 1) + 2);
  code_t x1 = match[0], y1 = match[1];
  code_t x2 = x1 + 1, y2 = y1 + 1;
  for (code_t i = 2; i < num; i += 2) {
    if (x2 == match[i] && y2 == match[i + 1]) {
      ++x2;
      ++y2;
      continue;
    }
    result.emplace_back(prefix + x1, prefix + y1, x2 - x1);
    x1 = match[i];
    y1 = match[i + 1];
    x2 = x1 + 1;
    y2 = y1 + 1;
  }
  result.emplace_back(prefix + x1, prefix + y1, x2 - x1);
  if (suffix > 0)
    result.emplace_back(prefix + n, prefix + m, suffix);
  // deallocate memory
  free(left); left = 0;
  free(right); right = 0;
  free(match); match = 0;
  return result;
}

template <typename T>
void snake(
    const T* a, code_t a_start, code_t a_end,
    const T* b, code_t b_start, code_t b_end,
    int* down, int* up, std::vector<seq>& result
) {
  if (a_start == a_end || b_start == b_end)
    return;
  int n = a_end - a_start;
  int m = b_end - b_start;
  if (n > m) {
    auto p = std::search(a + a_start, a + a_end, b + b_start, b + b_end);
    if (p - a < a_end) {
      result.emplace_back(p - a, b_start, m);
      return;
    }
    if (m == 1) return;
  } else {
    auto p = std::search(b + b_start, b + b_end, a + a_start, a + a_end);
    if (p - b < b_end) {
      result.emplace_back(a_start, p - b, n);
      return;
    }
    if (n == 1) return;
  }
  int d_max = (m + n + 1) / 2;
  int len = d_max * 2;
  for (int i = 0; i < len; ++i) {
    down[i] = -1;
    up  [i] = -1;
  }
  down[d_max + 1] = 0;
  up  [d_max + 1] = 0;
  int delta = n - m;
  bool front = (delta % 2 != 0);
  int x = 0, y = 0, d1 = 0, d2 = 0, u1 = 0, u2 = 0, k1 = 0, k2 = 0;
  for (int d = 0; d < d_max; ++d) {
    // forward path
    for (int k = -d + d1; k <= d - d2; k += 2) {
      k1 = d_max + k;
      if (k == -d || (k != d && down[k1 - 1] < down[k1 + 1]))
        x = down[k1 + 1];
      else
        x = down[k1 - 1] + 1;
      y = x - k;
      while (x < n && y < m && a[a_start + x] == b[b_start + y]) {
        ++x;
        ++y;
      }
      down[k1] = x;
      if (x > n)  { d2 += 2; continue; }
      if (y > m)  { d1 += 2; continue; }
      if (front)  {
        k2 = d_max + delta - k;
        if (k2 >= 0 && k2 < len && x >= n - up[k2]) {
          shortest_edit_script<T>(a, a_start, a_start + x, b, b_start, b_start + y, down, up, result);
          shortest_edit_script<T>(a, a_start + x, a_end, b, b_start + y, b_end, down, up, result);
          return;
        }
      }
    }
    // reverse path
    for (int k = -d + u1; k <= d - u2; k += 2) {
      k2 = d_max + k;
      if (k == -d || (k != d && up[k2 - 1] < up[k2 + 1]))
        x = up[k2 + 1];
      else
        x = up[k2 - 1] + 1;
      y = x - k;
      while (x < n && y < m && a[a_end - x - 1] == b[b_end - y - 1]) {
        ++x;
        ++y;
      }
      up[k2] = x;
      if (x > n)  { u2 += 2; continue; }
      if (y > m)  { u1 += 2; continue; }
      if (!front) {
        k1 = d_max + delta - k;
        if (k1 >= 0 && k1 < len && down[k1] >= n - x) {
          x = down[k1];
          y = x + k - delta;
          shortest_edit_script<T>(a, a_start, a_start + x, b, b_start, b_start + y, down, up, result);
          shortest_edit_script<T>(a, a_start + x, a_end, b, b_start + y, b_end, down, up, result);
          return;
        }
      }
    }
  }
}

template <typename T>
void shortest_edit_script(
    const T* a, code_t a_start, code_t a_end, 
    const T* b, code_t b_start, code_t b_end, 
    int* down, int* up, std::vector<seq>& result
) {
  code_t start = a_start, end = a_end;
  // trim off common prefix
  while (a_start < a_end && b_start < b_end && a[a_start] == b[b_start]) {
    ++a_start;
    ++b_start;
  }
  // trim off common suffix
  while (a_start < a_end && b_start < b_end && a[a_end - 1] == b[b_end - 1]) {
    --a_end;
    --b_end;
  }
  code_t n = a_start - start;
  if (n)  
    result.emplace_back(start, b_start - n, n);
  snake<T>(a, a_start, a_end, b, b_start, b_end, down, up, result);
  n = end - a_end;
  if (n)
    result.emplace_back(a_end, b_end, n);
}

template <typename T>
std::vector<seq> lcs_myers(const T* a, code_t n, const T* b, code_t m) {
  std::vector<seq> result;
  if (n == 0 || m == 0) return result;
  code_t c = n + m + 1;
  int* down = (int*) malloc((c << 1) * sizeof(int));
  if (! down)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  int* up = down + c;
  shortest_edit_script<T>(a, 0, n, b, 0, m, down, up, result);
  free(down); down = 0; up = 0;
  return result;
}

template <typename T>
seq find_longest_match(const T* a, code_t n, const T* b, code_t m) {
  if (n < m) {
    seq result = find_longest_match<T>(b, m, a, n);
    std::swap(std::get<0>(result), std::get<1>(result));
    return result;
  }
  if (m == 0)
    return {0, 0, 0};
  code_t x = 0, y = 0, len = 0;
  code_t* dp = (code_t*) malloc((m + 1) * sizeof(code_t));
  if (! dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  memset(dp, 0, (m + 1) * sizeof(code_t));
  for (int i = n - 1; i >= 0; --i) {
    for (int j = 0; j < m; ++j) {
      if (a[i] == b[j]) {
        dp[j] = dp[j + 1] + 1;
        if (dp[j] > len) {
          len = dp[j];
          x   = i;
          y   = j;
        }
      } else
        dp[j] = 0;
    }
  }
  free(dp); dp = 0;
  return {x, y, len};
}

template <typename T>
seq find_longest_match_diag(const T* a, code_t n, const T* b, code_t m) {
  if (n == 0 || m == 0)
    return {0, 0, 0};
  code_t k1, k2, x = 0, y = 0, len = 0, temp = 0;
  for (code_t i = 0; i < n; ++i) {
    // Reduce some unnecessary comparisons
    if (m == len || n <= i + len)
      break;
    k1 = i; k2 = 0; len = 0;
    while (k1 < n && k2 < m) {
      if (a[k1] == b[k2]) {
        ++len;
        if (len > temp) { x = k1; y = k2; temp = len; }
      } else  
        len = 0;
      ++k1; ++k2;
    }
  }
  for (code_t j = 1; j < m; ++j) {
    // Reduce some unnecessary comparisons
    if (n == len || m <= j + len)
      break;
    k1 = 0; k2 = j; len = 0;
    while (k1 < n && k2 < m) {
      if (a[k1] == b[k2]) {
        ++len;
        if (len > temp) { x = k1; y = k2; temp = len; }
      } else
        len = 0;
      ++k1; ++k2;
    }
  }
  return {x + 1 - len, y + 1 - len, len};
}

template <typename T>
code_t edit_distance(const T* a, code_t n, const T* b, code_t m) {
  if (n < m)
    return edit_distance<T>(b, m, a, n);
  // trim off the matching items at the beginning
  while (m > 0 && *a == *b) {
    ++a;
    ++b;
    --n;
    --m;
  }
  // trim off the matching items at the end
  while (m > 0 && a[n - 1] == b[m - 1]) {
    --n;
    --m;
  }
  if (m == 0)
    return n;
  code_t cost, temp, top_left;
  code_t* dp = (code_t*) malloc((m + 1) * sizeof(code_t));
  if (! dp)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (code_t i = 0; i <= m; ++i)
    dp[i] = i;
  for (code_t i = 1; i <= n; ++i) {
    *dp = i;
    top_left = i - 1;
    for (code_t j = 1; j <= m; ++j) {
      temp = dp[j];
      cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
      dp[j] = std::min(std::min(dp[j], dp[j - 1]) + 1, top_left + cost);
      top_left = temp;
    }
  }
  cost = dp[m];
  free(dp); dp = 0;
  return cost;
}

// Implementation of bounded Levenshtein distance (Ukkonen)
template <typename T>
int edit_distance_k(const T* a, int n, const T* b, int m, int k) {
  if (n > m)
    return edit_distance_k<T>(b, m, a, n, k);
  // trim off the matching items at the beginning
  while (n > 0 && *a == *b) {
    ++a;
    ++b;
    --n;
    --m;
  }
  // trim off the matching items at the end
  while (n > 0 && a[n - 1] == b[m - 1]) {
    --n;
    --m;
  }
  k = std::min(k, m);
  if (n == 0)
    return k;
  int d_len = m - n;
  if (k < d_len)
    return k;
  int ZERO_K = std::min(k, n) / 2 + 2;
  int array_len = d_len + ZERO_K * 2 + 2;
  int* current_row  = (int*) malloc(sizeof(int) * array_len);
  int* next_row     = (int*) malloc(sizeof(int) * array_len);
  if (! current_row || ! next_row)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  for (int i = 0; i < array_len; ++i) {
    current_row[i]  = -1;
    next_row[i]     = -1;
  }
  int i = 0, condition_row = d_len + ZERO_K; 
  int end_max = condition_row * 2;
  int start, end, previous_cell, current_cell, next_cell;
  do {
    ++i;
    std::swap(current_row, next_row);
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
    for (int j = start, row_index = start + ZERO_K; j < end; ++j, ++row_index) {
      previous_cell = current_cell;
      current_cell  = next_cell;
      next_cell = current_row[row_index + 1];
      int t = std::max(std::max(previous_cell, current_cell + 1), next_cell + 1);
      while (t < n && t + j < m && a[t] == b[t + j])
        ++t;
      next_row[row_index] = t;
    }
  } while (next_row[condition_row] < n && i <= k);
  free(current_row); current_row = 0;
  free(next_row); next_row = 0;
  return i - 1;
}

}
#endif

