# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-greentea STATIC EXCLUDE_FROM_ALL)
target_include_directories(mbed-greentea
    PUBLIC
        .
        greentea-client
)

target_sources(mbed-greentea
    PRIVATE
        source/greentea_metrics.cpp
        source/greentea_test_env.cpp
)

target_link_libraries(mbed-greentea PUBLIC mbed-core-flags)
