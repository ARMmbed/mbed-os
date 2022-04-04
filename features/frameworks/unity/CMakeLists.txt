# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-unity
    PUBLIC
        .
        unity
)

target_sources(mbed-unity
    PRIVATE
        source/unity.c
)

target_link_libraries(mbed-unity PUBLIC mbed-core-flags mbed-utest)
