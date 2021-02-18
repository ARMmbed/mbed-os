# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-f413zh INTERFACE)

target_sources(mbed-nucleo-f413zh
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-f413zh
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-f413zh INTERFACE mbed-stm32f413xh)
