# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-wio-emw3166 INTERFACE)

target_sources(mbed-wio-emw3166
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-wio-emw3166
    INTERFACE
        .
)

target_link_libraries(mbed-wio-emw3166 INTERFACE mbed-wiced mbed-stm32f412xg)
