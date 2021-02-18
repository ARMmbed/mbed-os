# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-f429zi INTERFACE)

target_sources(mbed-disco-f429zi
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-f429zi
    INTERFACE
        .
)

target_link_libraries(mbed-disco-f429zi INTERFACE mbed-stm32f429xi)
