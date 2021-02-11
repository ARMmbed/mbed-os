# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lis2dh12 INTERFACE)

target_include_directories(mbed-lis2dh12
    INTERFACE
        .
)

target_sources(mbed-lis2dh12
    INTERFACE
        lis2dh12_platform_apollo3.c
        lis2dh12_reg.c
)

target_link_libraries(mbed-lis2dh12 INTERFACE mbed-apollo3)
