# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f446re INTERFACE)

target_sources(mbed-nucleo-f446re
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f446re
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f446re INTERFACE mbed-stm32f446xe)
