# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-tapit-module-1 INTERFACE)

target_sources(mbed-tapit-module-1
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-tapit-module-1
    INTERFACE
        .
)

target_link_libraries(mbed-tapit-module-1 INTERFACE mbed-stm32f767xi)
