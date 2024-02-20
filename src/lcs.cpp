/**
 * Copyright (c) 2023-present, Zejun Wang.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "lcs.h"

namespace lcs {

code_t strlcs_len(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  code_t l = lcs_len<code_t>(acodes, n, bcodes, m);
  free(acodes); acodes = 0; bcodes = 0;
  return l;
}

code_t strlcs_len(const std::string& a, const std::string& b) {
  return strlcs_len(a.c_str(), a.size(), b.c_str(), b.size());
}

code_t strlcs_len_map(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  code_t l = lcs_len_map<code_t>(acodes, n, bcodes, m);
  free(acodes); acodes = 0; bcodes = 0;
  return l;
}

code_t strlcs_len_map(const std::string& a, const std::string& b) {
  return strlcs_len_map(a.c_str(), a.size(), b.c_str(), b.size());
}

std::vector<seq> strlcs_dp(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  std::vector<seq> result = std::move(lcs_dp<code_t>(acodes, n, bcodes, m));
  free(acodes); acodes = 0; bcodes = 0;
  return result;
}

std::vector<seq> strlcs_dp(const std::string& a, const std::string& b) {
  return strlcs_dp(a.c_str(), a.size(), b.c_str(), b.size());
}

std::vector<seq> strlcs_hirschberg(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  std::vector<seq> result = std::move(lcs_hirschberg<code_t>(acodes, n, bcodes, m));
  free(acodes); acodes = 0; bcodes = 0;
  return result;
}

std::vector<seq> strlcs_hirschberg(const std::string& a, const std::string& b) {
  return strlcs_hirschberg(a.c_str(), a.size(), b.c_str(), b.size());
}

std::vector<seq> strlcs_myers(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  std::vector<seq> result = std::move(lcs_myers<code_t>(acodes, n, bcodes, m)); 
  free(acodes); acodes = 0; bcodes = 0;
  return result;
}

std::vector<seq> strlcs_myers(const std::string& a, const std::string& b) {
  return strlcs_myers(a.c_str(), a.size(), b.c_str(), b.size());
}

seq longest_match(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  seq result = find_longest_match<code_t>(acodes, n, bcodes, m);
  free(acodes); acodes = 0; bcodes = 0;
  return result;
}

seq longest_match(const std::string& a, const std::string& b) {
  return longest_match(a.c_str(), a.size(), b.c_str(), b.size());
}

seq longest_match_diag(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  seq result = find_longest_match_diag<code_t>(acodes, n, bcodes, m);
  free(acodes); acodes = 0; bcodes = 0;
  return result;
}

seq longest_match_diag(const std::string& a, const std::string& b) {
  return longest_match_diag(a.c_str(), a.size(), b.c_str(), b.size());
}

code_t str_edit_distance(const char* a, code_t alen, const char* b, code_t blen) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  code_t d = edit_distance<code_t>(acodes, n, bcodes, m);
  free(acodes); acodes = 0; bcodes = 0;
  return d;
}

code_t str_edit_distance(const std::string& a, const std::string& b) {
  return str_edit_distance(a.c_str(), a.size(), b.c_str(), b.size());
}

code_t str_edit_distance_k(const char* a, code_t alen, const char* b, code_t blen, code_t k) {
  code_t* acodes = (code_t*) malloc((alen + blen) * sizeof(code_t));
  if (! acodes)
    err(__FILE__, __LINE__, "memory reallocation failed\n");
  code_t* bcodes = acodes + alen;
  code_t n = decode(a, alen, acodes);
  code_t m = decode(b, blen, bcodes);
  code_t d = static_cast<code_t>(edit_distance_k<code_t>(
      acodes, static_cast<int>(n), bcodes, static_cast<int>(m), static_cast<int>(k)));
  free(acodes); acodes = 0; bcodes = 0;
  return d;
}

code_t str_edit_distance_k(const std::string& a, const std::string& b, code_t k) {
  return str_edit_distance_k(a.c_str(), a.size(), b.c_str(), b.size(), k);
}

}

