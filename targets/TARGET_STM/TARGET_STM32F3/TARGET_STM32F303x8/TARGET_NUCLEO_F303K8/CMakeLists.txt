# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f303k8 INTERFACE)

target_include_directories(mbed-nucleo-f303k8
    INTERFACE
        .
)

target_sources(mbed-nucleo-f303k8
    INTERFACE
        PeripheralPins.c
)

target_link_libraries(mbed-nucleo-f303k8 INTERFACE mbed-stm32f303x8)
