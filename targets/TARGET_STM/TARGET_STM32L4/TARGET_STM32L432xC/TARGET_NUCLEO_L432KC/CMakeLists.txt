# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l432kc INTERFACE)

target_sources(mbed-nucleo-l432kc
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l432kc
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l432kc INTERFACE mbed-stm32l432xc)
