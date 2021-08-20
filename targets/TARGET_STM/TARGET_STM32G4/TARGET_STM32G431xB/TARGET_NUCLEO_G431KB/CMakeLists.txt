# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g431kb INTERFACE)

target_sources(mbed-nucleo-g431kb
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-g431kb
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g431kb INTERFACE mbed-stm32g431xb)
