# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("FEATURE")

add_subdirectory(frameworks/mbed-client-randlib)
add_subdirectory(frameworks/mbed-trace)
add_subdirectory(frameworks/mbed-client-cli)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
