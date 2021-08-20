# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-wl55jc INTERFACE)

target_sources(mbed-nucleo-wl55jc
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-wl55jc
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-wl55jc INTERFACE mbed-stm32wl55xc)
