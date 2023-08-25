/**
 * Copyright (c) 2023-present, Zejun Wang
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#ifndef FASTLCS_H
#define FASTLCS_H

#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#if __cplusplus >= 201402L
#include "flat_hash_map/bytell_hash_map.hpp"
#else
#include "flat_hash_map/unordered_map.hpp"
#endif

using namespace std;

#if __cplusplus >= 201703L

void split_chars(string_view s, vector<string_view>& res);

int lcs_length(string_view s1, string_view s2, bool transform = true);

int lcs(string_view s1, string_view s2, vector<tuple<int, int, int>>& pos);

vector<tuple<int, int, int>> lcs(string_view s1, string_view s2);

int lcsubstr_length(string_view s1, string_view s2);

tuple<int, int, int> lcsubstr(string_view s1, string_view s2);

int edit_distance(string_view s1, string_view s2);

#else

void split_chars(const string& s, vector<string>& res);

int lcs_length(const string& s1, const string& s2, bool transform = true);

int lcs(const string& s1, const string& s2, vector<tuple<int, int, int>>& pos);

vector<tuple<int, int, int>> lcs(const string& s1, const string& s2);

int lcsubstr_length(const string& s1, const string& s2);

tuple<int, int, int> lcsubstr(const string& s1, const string& s2);

int edit_distance(const string& s1, const string& s2);

#endif

vector<int> lcs_length_group(const string& q, const vector<string>& v,
            bool transform = true, int num_threads = 0);

vector<int> lcsubstr_length_group(const string& q, const vector<string>& v,
            int num_threads = 0);

vector<int> edit_distance_group(const string& q, const vector<string>& v,
            int num_threads = 0);

#endif
