#
# Copyright (c) 2023-present, Zejun Wang.
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

import _fastlcs

def lcs_len_dp(s1: str, s2: str) -> int:
    return _fastlcs.lcs_len_dp(s1, len(s1), s2, len(s2))

def lcs_len_map(s1: str, s2: str) -> int:
    return _fastlcs.lcs_len_map(s1, len(s1), s2, len(s2))

def lcs_dp(s1: str, s2: str):
    return _fastlcs.lcs_dp(s1, len(s1), s2, len(s2))

def lcs_hirschberg(s1: str, s2: str):
    return _fastlcs.lcs_hirschberg(s1, len(s1), s2, len(s2))

def lcsubstr_dp(s1: str, s2: str):
    return _fastlcs.lcsubstr_dp(s1, len(s1), s2, len(s2))

def lcsubstr_diag(s1: str, s2: str):
    return _fastlcs.lcsubstr_diag(s1, len(s1), s2, len(s2))

def edit_distance(s1: str, s2: str) -> int:
    return _fastlcs.edit_distance(s1, len(s1), s2, len(s2))

def edit_distance_k(s1: str, s2: str, k: int) -> int:
    return _fastlcs.edit_distance_k(s1, len(s1), s2, len(s2), k)

