#!/usr/bin/env python

"""
# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
"""

import os

if __name__ == "__main__":
    os.system("cmake -DUNITTESTS=ON -S . -B cmake_build -GNinja")
    os.system("cmake --build cmake_build")
    os.chdir("cmake_build")
    os.system("ctest -V")
