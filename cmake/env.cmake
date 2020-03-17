# SPDX-License-Identifier: Apache-2.0

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Project setup
enable_language(C CXX ASM)

# TODO: @mbed-os-tools This needs to come from tools
set_property(GLOBAL PROPERTY MBED_OS_COMPONENTS "")

