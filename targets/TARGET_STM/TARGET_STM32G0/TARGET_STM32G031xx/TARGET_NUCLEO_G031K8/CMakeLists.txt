# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-g031k8 INTERFACE)

target_sources(mbed-nucleo-g031k8
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-g031k8
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-g031k8 INTERFACE mbed-stm32g031xx)
