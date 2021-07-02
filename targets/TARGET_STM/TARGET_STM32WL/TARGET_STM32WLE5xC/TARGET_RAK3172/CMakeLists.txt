# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-rak3172 INTERFACE)

target_sources(mbed-rak3172
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-rak3172
    INTERFACE
        .
)

target_link_libraries(mbed-rak3172 INTERFACE mbed-stm32wle5xc)
