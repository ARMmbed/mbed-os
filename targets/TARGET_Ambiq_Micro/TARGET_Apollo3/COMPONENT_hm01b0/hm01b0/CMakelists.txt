# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-hm01b0 INTERFACE)

target_include_directories(mbed-hm01b0
    INTERFACE
        .
)

target_sources(mbed-hm01b0
    INTERFACE
        HM01B0.c
)

target_link_libraries(mbed-hm01b0 INTERFACE mbed-apollo3)
