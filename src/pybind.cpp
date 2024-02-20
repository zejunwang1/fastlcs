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

namespace py = pybind11;

using POS = std::vector<lcs::seq>;
PYBIND11_MAKE_OPAQUE(POS);

PYBIND11_MODULE(fastlcs, m) {
  m.doc() = "An effective tool for solving LCS problems.";

  py::bind_vector<POS>(m, "POS");
  m.def("lcs_len", &lcs::lcs_len<wchar_t>);
  m.def("lcs_len_map", &lcs::lcs_len_map<wchar_t>);
  m.def("lcs_dp", &lcs::lcs_dp<wchar_t>);
  m.def("lcs_hirschberg", &lcs::lcs_hirschberg<wchar_t>);
  m.def("lcs_myers", &lcs::lcs_myers<wchar_t>);
  m.def("longest_match", &lcs::find_longest_match<wchar_t>);
  m.def("longest_match_diag", &lcs::find_longest_match_diag<wchar_t>);
  m.def("edit_distance", &lcs::edit_distance<wchar_t>);
  m.def("edit_distance_k", &lcs::edit_distance_k<wchar_t>);
}

