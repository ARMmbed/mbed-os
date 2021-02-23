# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32630fthr INTERFACE)

target_include_directories(mbed-max32630fthr 
    INTERFACE
        .
)

target_sources(mbed-max32630fthr
    INTERFACE
        low_level_init.c
)

target_link_libraries(mbed-max32630fthr INTERFACE mbed-max32630)
