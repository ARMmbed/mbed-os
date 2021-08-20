# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-flash-cmsis-algo INTERFACE)

target_include_directories(mbed-flash-cmsis-algo
    INTERFACE
        .
)

target_sources(mbed-flash-cmsis-algo
    INTERFACE
        flash_common_algo.c
)
