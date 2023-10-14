/**
 * Copyright (c) 2023-present, Zejun Wang.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <lcs.h>
#include <tuple>

namespace py = pybind11;
using Tuple = tuple<uint32_t, uint32_t, uint32_t>;
using POS   = vector<Tuple>;
PYBIND11_MAKE_OPAQUE(POS);

PYBIND11_MODULE(_fastlcs, m) {
  m.doc() = "An effective tool for solving LCS problems.";
  py::bind_vector<POS>(m, "POS");
  
  m.def("lcs_len_dp", &fastlcs::lcs_len_dp_impl<wchar_t>);
  m.def("lcs_len_map", &fastlcs::lcs_len_map_impl<wchar_t>);
  m.def(
    "lcs_dp",
    [](const wchar_t* a, uint32_t a_len, const wchar_t* b, uint32_t b_len) {
      uint32_t size = 0;
      auto result = fastlcs::lcs_dp_impl <wchar_t> (a, a_len, b, b_len, size);
      POS pos;
      if (size)
        pos.reserve(size);
      for (uint32_t i = 0; i < size; i++)
        pos.emplace_back(result[i].b1, result[i].b2, result[i].len);
      if (result)
        free(result);
      return pos;
    }
  );
  m.def(
    "lcs_hirschberg",
    [](const wchar_t* a, uint32_t a_len, const wchar_t* b, uint32_t b_len) {
      uint32_t size = 0;
      auto result = fastlcs::lcs_hirschberg_impl <wchar_t> (a, a_len, b, b_len, size);
      POS pos;
      if (size)
        pos.reserve(size);
      for (uint32_t i = 0; i < size; i++)
        pos.emplace_back(result[i].b1, result[i].b2, result[i].len);
      if (result)
        free(result);
      return pos;
    }
  );
  m.def(
    "lcsubstr_dp",
    [](const wchar_t* a, uint32_t a_len, const wchar_t* b, uint32_t b_len) {
      auto result = fastlcs::lcsubstr_dp_impl <wchar_t> (a, a_len, b, b_len);
      return Tuple(result.b1, result.b2, result.len);
    }
  );
  m.def(
    "lcsubstr_diag",
    [](const wchar_t* a, uint32_t a_len, const wchar_t* b, uint32_t b_len) {
      auto result = fastlcs::lcsubstr_diag_impl <wchar_t> (a, a_len, b, b_len);
      return Tuple(result.b1, result.b2, result.len);
    }
  );
  m.def("edit_distance", &fastlcs::edit_distance_impl<wchar_t>);
  m.def("edit_distance_k", &fastlcs::edit_distance_k_impl<wchar_t>);
}

