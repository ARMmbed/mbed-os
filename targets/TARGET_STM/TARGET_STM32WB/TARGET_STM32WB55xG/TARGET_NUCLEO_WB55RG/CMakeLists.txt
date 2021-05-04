# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-wb55rg INTERFACE)

target_sources(mbed-nucleo-wb55rg
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-wb55rg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-wb55rg INTERFACE mbed-stm32wb55xg)
