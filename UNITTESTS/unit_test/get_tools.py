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


GET UNIT TEST TOOLS
"""

import platform

from .utils import is_tool
from .settings import MAKE_PROGRAMS, \
                      CXX_COMPILERS, \
                      C_COMPILERS, \
                      GCOV_PROGRAMS

def _get_program(programs, default=None):
    if programs is None:
        programs = []

    for program in programs:
        if is_tool(program):
            return program

    return default

def get_make_tool():
    """
    Get make program
    """

    default_make = "mingw32-make" if (platform.system() == "Windows") \
                             else "make"

    return _get_program(MAKE_PROGRAMS, default=default_make)

def get_cmake_tool():
    """
    Get cmake program
    """

    return _get_program(["cmake"])

def get_valgrind_tool():
    """
    Get Valgrind program
    """

    return _get_program(["valgrind"])

def get_cxx_tool():
    """
    Get C++ compiler
    """

    return _get_program(CXX_COMPILERS, "g++")

def get_c_tool():
    """
    Get C compiler
    """

    return _get_program(C_COMPILERS, "gcc")

def get_gcov_program():
    """
    Get gcov program
    """

    return _get_program(GCOV_PROGRAMS, "gcov")

def get_gcovr_program():
    """
    Get covr program
    """

    return _get_program(["gcovr"])
