# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h743zi INTERFACE)

target_sources(mbed-nucleo-h743zi
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-h743zi
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h743zi INTERFACE mbed-stm32743xi)
