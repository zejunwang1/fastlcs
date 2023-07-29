/**
 * Copyright (c) 2023-present, Zejun Wang
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#if __cplusplus >= 201402L
#include "flat_hash_map/bytell_hash_map.hpp"
#else
#include "flat_hash_map/unordered_map.hpp"
#endif

#include <iostream>
#include <ctime>

using namespace std;

#if __cplusplus >= 201703L
void split_chars(string_view s, vector<string_view>& res) {
#else
void split_chars(const string& s, vector<string>& res) {
#endif
  size_t left = 0;
  size_t right = 1;
  res.reserve(s.size());
  while (right < s.size()) {
    if ((s[right] & 0xC0) != 0x80) {
      res.emplace_back(s.substr(left, right - left));
      left = right;
    }
    right++;
  }
  res.emplace_back(s.substr(left));
}

#if __cplusplus >= 201703L
int lcs_length_dp(const vector<string_view>& v1, const vector<string_view>& v2) {
#else
int lcs_length_dp(const vector<string>& v1, const vector<string>& v2) {
#endif
  // Space complexity O(min(m, n))
  if (v1.size() < v2.size())
    return lcs_length_dp(v2, v1);
  
  int temp = 0;
  int top_left = 0;
  auto m = v1.size();
  auto n = v2.size();
  vector<int> dp(n + 1);
  
  for (auto i = 1; i <= m; i++) {
    top_left = 0;
    for (auto j = 1; j <= n; j++) {
      temp = dp[j];
      if (v1[i - 1] == v2[j - 1])
        dp[j] = top_left + 1;
      else
        dp[j] = max(dp[j - 1], dp[j]);
      top_left = temp;
    }
  }
  
  return dp[n];
}

// Longest Increasing Subsequence
// Time complexity O(nlogn)
int LIS(const vector<int>& a) {
  if (a.empty())
    return 0;
  
  int len = 0;
  vector<int> b(a.size() + 1);
  for (const auto& i : a) {
    if (b[len] < i)
      b[++len] = i;
    else
      *lower_bound(b.begin() + 1, b.begin() + len + 1, i) = i;
  }

  return len;
}

#if __cplusplus >= 201703L
int lcs_length_transform(const vector<string_view>& v1, const vector<string_view>& v2) {
#else
int lcs_length_transform(const vector<string>& v1, const vector<string>& v2) {
#endif
  if (v1.size() < v2.size())
    return lcs_length_transform(v2, v1);
  
#if __cplusplus >= 201703L
  ska::bytell_hash_map<string_view, vector<int>> h;
#elif __cplusplus >= 201402L
  ska::bytell_hash_map<string, vector<int>> h;
#else
  ska::unordered_map<string, vector<int>> h;
#endif
  
  for (auto& s : v2) {
    if (!h.count(s)) {
      vector<int> val;
      h[s] = val;
    }
  }
  
  for (auto i = v1.size(); i > 0; i--) {
    auto s = v1[i - 1];
    if (h.count(s))
      h[s].emplace_back(i);
  }
  
  vector<int> a;
  a.reserve(v2.size() * 3);
  for (auto& s : v2)
    for (auto& i : h[s])
      a.emplace_back(i);
  
  return LIS(a);
}

#if __cplusplus >= 201703L
int lcs_length(string_view s1, string_view s2, bool transform) {
#else
int lcs_length(const string& s1, const string& s2, bool transform) {
#endif
  if (s1.empty() || s2.empty())
    return 0;
  
#if __cplusplus >= 201703L
  vector<string_view> v1;
  vector<string_view> v2;
#else
  vector<string> v1;
  vector<string> v2;
#endif
  split_chars(s1, v1);
  split_chars(s2, v2);
  
  if (transform)
    return lcs_length_transform(v1, v2);

  return lcs_length_dp(v1, v2);
}

#if __cplusplus >= 201703L
int lcs(string_view s1, string_view s2, vector<tuple<int, int, int>>& pos) {
#else
int lcs(const string& s1, const string& s2, vector<tuple<int, int, int>>& pos) {
#endif
  if (s1.empty() || s2.empty())
    return 0;
  
#if __cplusplus >= 201703L
  vector<string_view> v1;
  vector<string_view> v2;
#else
  vector<string> v1;
  vector<string> v2;
#endif
  split_chars(s1, v1);
  split_chars(s2, v2);
  
  auto m = v1.size();
  auto n = v2.size();
  vector<vector<int>> dp(m + 1, vector<int>(n + 1));
  for (auto i = 1; i <= m; i++) {
    for (auto j = 1; j <= n; j++) {
      if (v1[i - 1] == v2[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1;
      else
        dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
    }
  }
  
  auto len = dp[m][n];
  if (len == 0)
    return 0;
  
  // Location of LCS
  auto i = m;
  auto j = n;
  vector<pair<int, int>> loc;
  loc.reserve(len);
  while (i > 0 && j > 0) {
    if (v1[i - 1] == v2[j - 1]) {
      loc.emplace_back(i, j);
      i--;
      j--;
    } else if (dp[i][j] == dp[i][j - 1])
      j--;
    else
      i--;
  }
  
  auto e1 = loc[0].first;
  auto e2 = loc[0].second;
  auto b1 = e1 - 1;
  auto b2 = e2 - 1;
  pos.reserve(len);
  for (auto i = 1; i < loc.size(); i++) {
    if (b1 == loc[i].first && b2 == loc[i].second) {
      b1 -= 1;
      b2 -= 1;
    } else {
      pos.emplace_back(b1, b2, e1 - b1);
      e1 = loc[i].first;
      e2 = loc[i].second;
      b1 = e1 - 1;
      b2 = e2 - 1;
    }
  }
  pos.emplace_back(b1, b2, e1 - b1);
  reverse(pos.begin(), pos.end());
  
  return len;   
}

#if __cplusplus >= 201703L
vector<tuple<int, int, int>> lcs(string_view s1, string_view s2) {
#else
vector<tuple<int, int, int>> lcs(const string& s1, const string& s2) {
#endif
  vector<tuple<int, int, int>> pos;
  lcs(s1, s2, pos);
  return pos;
}

#if __cplusplus >= 201703L
int lcsubstr_length(string_view s1, string_view s2) {
#else
int lcsubstr_length(const string& s1, const string& s2) {
#endif
  if (s1.empty() || s2.empty())
    return 0;
  
  if (s1.size() < s2.size())
    return lcsubstr_length(s2, s1);
  
#if __cplusplus >= 201703L
  vector<string_view> v1;
  vector<string_view> v2;
#else
  vector<string> v1;
  vector<string> v2;
#endif
  split_chars(s1, v1);
  split_chars(s2, v2);
  
  int len = 0;
  auto m = v1.size();
  auto n = v2.size();
  vector<int> dp(n + 1);
  for (auto i = 1; i <= m; i++) {
    for (auto j = n; j > 0; j--) {
      if (v1[i - 1] == v2[j - 1]) {
        dp[j] = dp[j - 1] + 1;
        len = max(len,  dp[j]);
      } else
        dp[j] = 0;
    }
  }

  return len;
}

#if __cplusplus >= 201703L
tuple<int, int, int> lcsubstr(string_view s1, string_view s2) {
#else
tuple<int, int, int> lcsubstr(const string& s1, const string& s2) {
#endif
  if (s1.empty() || s2.empty())
    return {0, 0, 0};
  
  if (s1.size() < s2.size())
    return lcsubstr(s2, s1);
  
#if __cplusplus >= 201703L
  vector<string_view> v1;
  vector<string_view> v2;
#else
  vector<string> v1;
  vector<string> v2;
#endif
  split_chars(s1, v1);
  split_chars(s2, v2);
  
  auto m = v1.size();
  auto n = v2.size();
  vector<int> dp(n + 1);
  int e1 = 0, e2 = 0, len = 0;

  for (auto i = 1; i <= m; i++) {
    for (auto j = n; j > 0; j--) {
      if (v1[i - 1] == v2[j - 1]) {
        dp[j] = dp[j - 1] + 1;
        if (dp[j] > len) {
          len = dp[j];
          e1 = i;
          e2 = j;
        }      
      } else
        dp[j] = 0;
    }
  }
  
  return {len, e1 - len, e2 - len};
}

#if __cplusplus >= 201703L
int levenshtein_distance(const vector<string_view>& v1, const vector<string_view>& v2) {
#else
int levenshtein_distance(const vector<string>& v1, const vector<string>& v2) {
#endif
  if (v1.empty())
    return v2.size();
  if (v2.empty())
    return v1.size();
  if (v1.size() < v2.size())
    return levenshtein_distance(v2, v1);
  
  int cost = 0;
  int temp = 0;
  int top_left = 0;
  auto m = v1.size();
  auto n = v2.size();

  vector<int> dp(n + 1);
  for (auto j = 0; j <= n; j++)
    dp[j] = j;  
   
  for (auto i = 1; i <= m; i++) {
    top_left = i - 1;
    for (auto j = 1; j <= n; j++) {
      temp = dp[j];
      cost = (v1[i - 1] == v2[j - 1]) ? 0 : 1;
      dp[j] = min(dp[j], dp[j - 1]) + 1;
      dp[j] = min(dp[j], top_left + cost); 
      top_left = temp;
    }
  }
  return dp[n];
}

#if __cplusplus >= 201703L
int edit_distance(string_view s1, string_view s2) {
  vector<string_view> v1;
  vector<string_view> v2;
#else
int edit_distance(const string& s1, const string& s2) {
  vector<string> v1;
  vector<string> v2;
#endif
  split_chars(s1, v1);
  split_chars(s2, v2);
  
  return levenshtein_distance(v1, v2); 
}

void RunMultiThread(function<void(size_t, size_t)> func, size_t n, int num_threads) {
  vector<thread> threads;
  threads.reserve(static_cast<size_t>(num_threads));
  
  size_t start = 0, end = 0, step = ceil(n / float(num_threads));
  for (int i = 0; i < num_threads; i++) {
    end = start + step;
    if (end > n)
      end = n;
    threads.emplace_back(std::thread(func, start, end));
    start = end;
  }

  for (auto& t : threads)
    t.join();
}

vector<int> lcs_length_group(const string& q, const vector<string>& v, 
            bool transform, int num_threads) {
  if (v.empty())
    return {};
  
  auto n = v.size();
  vector<int> res(n);
  if (num_threads <= 1) {
    for (auto i = 0; i < n; i++)
      res[i] = lcs_length(q, v[i], transform);
    return res;
  }

  // Multithreading Implementation
#ifdef WITH_OMP
  #pragma omp parallel for num_threads(num_threads)
  for (auto i = 0; i < n; i++)
    res[i] = lcs_length(q, v[i], transform);
#else
  auto func = [&](size_t start_index, size_t end_index)
  {
    for (auto i = start_index; i < end_index; i++)
      res[i] = lcs_length(q, v[i], transform); 
  };
   
  RunMultiThread(func, n, num_threads);
#endif

  return res;
}

vector<int> lcsubstr_length_group(const string& q, const vector<string>& v, 
            int num_threads) {
  if (v.empty())
    return {};
  
  auto n = v.size();
  vector<int> res(n);
  if (num_threads <= 1) {
    for (auto i = 0; i < n; i++)
      res[i] = lcsubstr_length(q, v[i]);
    return res;
  }
  
  // Multithreading Implementation
#ifdef WITH_OMP
  #pragma omp parallel for num_threads(num_threads)
  for (auto i = 0; i < n; i++)
    res[i] = lcsubstr_length(q, v[i]);
#else
  auto func = [&](size_t start_index, size_t end_index)
  {
    for (auto i = start_index; i < end_index; i++)
      res[i] = lcsubstr_length(q, v[i]);
  };
  
  RunMultiThread(func, n, num_threads);
#endif
  
  return res;
}

vector<int> edit_distance_group(const string& q, const vector<string>& v, int num_threads) {
  if (v.empty())
    return {};
  
  auto n = v.size();
  vector<int> res(n);
  if (num_threads <= 1) {
    for (auto i = 0; i < n; i++)
      res[i] = edit_distance(q, v[i]);
    return res;
  }
  
  // Multithreading Implementation
#ifdef WITH_OMP
#pragma omp parallel for num_threads(num_threads)
  for (auto i = 0; i < n; i++)
    res[i] = edit_distance(q, v[i]);
#else
  auto func = [&](size_t start_index, size_t end_index)
  {
    for (auto i = start_index; i < end_index; i++)
      res[i] = edit_distance(q, v[i]);
  };
  
  RunMultiThread(func, n, num_threads);
#endif

  return res;
}

