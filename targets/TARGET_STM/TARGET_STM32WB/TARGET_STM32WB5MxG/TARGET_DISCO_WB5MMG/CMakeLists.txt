# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-wb5mmg INTERFACE)

target_sources(mbed-disco-wb5mmg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-wb5mmg
    INTERFACE
        .
)

target_link_libraries(mbed-disco-wb5mmg INTERFACE mbed-stm32wb5mxg)
