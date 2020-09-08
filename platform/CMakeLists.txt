# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("FEATURE")

add_subdirectory(cxxsupport)
add_subdirectory(mbed-trace)
add_subdirectory(randlib)
add_subdirectory(source)

target_include_directories(mbed-os
    PUBLIC
        include
        include/platform
        include/platform/internal
)
