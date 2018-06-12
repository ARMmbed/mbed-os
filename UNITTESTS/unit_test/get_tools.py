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
                      MAC_CXX_COMPILERS, \
                      MAC_C_COMPILERS, \
                      MAC_GCOV_PROGRAMS

def get_make_tool():
    """
    Get make program
    """

    for cmd in MAKE_PROGRAMS:
        if is_tool(cmd):
            return cmd

    return "mingw32-make" if (platform.system() == "Windows") \
                                else "make"

def get_cmake_tool():
    """
    Get cmake program
    """

    return "cmake"

def get_cxx_tool():
    """
    Get C++ compiler
    """

    if platform.system() == "Darwin":
        for cmd in MAC_CXX_COMPILERS:
            if is_tool(cmd):
                return cmd

    return "g++"

def get_c_tool():
    """
    Get C compiler
    """

    if platform.system() == "Darwin":
        for cmd in MAC_C_COMPILERS:
            if is_tool(cmd):
                return cmd

    return "gcc"

def get_gcov_program():
    """
    Get gcov program
    """

    if platform.system() == "Darwin":
        for cmd in MAC_GCOV_PROGRAMS:
            if is_tool(cmd):
                return cmd

    return "gcov"
