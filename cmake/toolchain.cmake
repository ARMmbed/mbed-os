# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/cmake/toolchains/${MBED_TOOLCHAIN}.cmake)

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Project setup
enable_language(C CXX ASM)
