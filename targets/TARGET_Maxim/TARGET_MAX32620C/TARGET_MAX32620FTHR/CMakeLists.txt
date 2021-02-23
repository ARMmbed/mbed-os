# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32620fthr INTERFACE)

target_include_directories(mbed-max32620fthr
    INTERFACE
        .
)

target_link_libraries(mbed-max32620fthr INTERFACE mbed-maxim-max32620c)
