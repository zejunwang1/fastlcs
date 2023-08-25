#
# Copyright (c) 2023-present, Zejun Wang
# All rights reserved.
#
# This source code is licensed under the MIT license found in the LICENSE file 
# in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pthread -march=native -funroll-loops")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -pthread -march=native -funroll-loops")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(fastlcs_lib src/fastlcs.cpp)
set_target_properties(fastlcs_lib PROPERTIES
    OUTPUT_NAME fastlcs
    PREFIX "lib")
add_library(fastlcs_shared_lib SHARED src/fastlcs.cpp)
set_target_properties(fastlcs_shared_lib PROPERTIES
    OUTPUT_NAME fastlcs
    PREFIX "lib")

add_executable(example example/example.cpp)
target_link_libraries(example fastlcs_lib)

