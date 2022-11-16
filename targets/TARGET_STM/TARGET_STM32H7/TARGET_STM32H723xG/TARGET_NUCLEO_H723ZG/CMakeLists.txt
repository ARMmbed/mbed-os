# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h723zg INTERFACE)

target_sources(mbed-nucleo-h723zg
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-h723zg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h723zg INTERFACE mbed-stm32h723xg)
