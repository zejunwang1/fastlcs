/**
 * Copyright (c) 2023-present, Zejun Wang
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>

#include "fastlcs.h"

namespace py = pybind11;

using POS = vector<tuple<int, int, int>>;

PYBIND11_MAKE_OPAQUE(POS);

POS lcs_bind(const string& s1, const string& s2) {
  POS pos;
  lcs(s1, s2, pos);
  return pos;
}

PYBIND11_MODULE(fastlcs, m) {

  m.doc() = "An efficient tool for solving LCS.";
  
  py::bind_vector<POS>(m, "POS");

  m.def("lcs_length", 
        &lcs_length, 
        py::arg("s1"), py::arg("s2"), py::arg("transform") = true
  );
  
  m.def("lcs",
        &lcs_bind,
        py::arg("s1"), py::arg("s2")
  );
  
  m.def("lcsubstr_length",
        &lcsubstr_length,
        py::arg("s1"), py::arg("s2")
  );
  
  m.def("lcsubstr",
        &lcsubstr,
        py::arg("s1"), py::arg("s2")
  );
  
  m.def("edit_distance",
        &edit_distance,
        py::arg("s1"), py::arg("s2")
  );
  
  m.def("lcs_length_group",
        &lcs_length_group,
        py::arg("s1"), py::arg("s2"), 
        py::arg("transform") = true,
        py::arg("num_threads") = 4
  );
  
  m.def("lcsubstr_length_group",
        &lcsubstr_length_group,
        py::arg("s1"), py::arg("s2"),
        py::arg("num_threads") = 4
  );
  
  m.def("edit_distance_group",
        &edit_distance_group,
        py::arg("s1"), py::arg("s2"),
        py::arg("num_threads") = 4
  );
}

