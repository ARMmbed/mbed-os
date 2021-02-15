# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sdt32620b INTERFACE)

target_include_directories(mbed-sdt32620b
    INTERFACE
        .
)

target_link_libraries(mbed-sdt32620b INTERFACE mbed-maxim-max32620c)
