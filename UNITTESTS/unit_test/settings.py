"""
Copyright (c) 2018, Arm Limited
SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


UNIT TEST SETTINGS
"""

CMAKE_GENERATORS = ["Unix Makefiles", "MinGW Makefiles", "Ninja"]

MAKE_PROGRAMS = ["gmake", "make", "mingw32-make", "ninja"]

DEFAULT_CMAKE_GENERATORS = {
    "gmake": "Unix Makefiles",
    "make": "Unix Makefiles",
    "mingw32-make": "MinGW Makefiles",
    "ninja": "Ninja"
}

COVERAGE_ARGS = ["html",
                  "xml",
                  "both"]

COVERAGE_OUTPUT_TYPES = ["html", "xml"]

CXX_COMPILERS = ["g++-6", "g++-8", "g++-7", "g++-5", "g++-4.9", "g++"]

C_COMPILERS = ["gcc-6", "gcc-8", "gcc-7", "gcc-5", "gcc-4.9", "gcc"]

GCOV_PROGRAMS = ["gcov-6", "gcov-8", "gcov-7", "gcov-5", "gcov-4.9", "gcov"]
