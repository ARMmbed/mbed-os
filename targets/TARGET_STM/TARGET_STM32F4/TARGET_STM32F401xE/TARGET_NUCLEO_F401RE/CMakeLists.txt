# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f401re INTERFACE)

target_include_directories(mbed-nucleo-f401re
    INTERFACE
        .
)

target_sources(mbed-nucleo-f401re
    INTERFACE
        PeripheralPins.c
)

target_link_libraries(mbed-nucleo-f401re INTERFACE mbed-stm32f401xe)
