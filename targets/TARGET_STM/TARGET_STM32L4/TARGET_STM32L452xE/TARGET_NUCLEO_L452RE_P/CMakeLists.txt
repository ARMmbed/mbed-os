# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l452re-p INTERFACE)

target_sources(mbed-nucleo-l452re-p
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l452re-p
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l452re-p INTERFACE mbed-stm32l452xe)
