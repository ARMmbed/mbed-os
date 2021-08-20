# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f091rc INTERFACE)

target_sources(mbed-nucleo-f091rc
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f091rc
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f091rc INTERFACE mbed-stm32f091xc)
