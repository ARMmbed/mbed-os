# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f413zh INTERFACE)

target_sources(nucleo-f413zh
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f413zh
    INTERFACE
        .
)

target_link_libraries(nucleo-f413zh INTERFACE mbed-stm32f413xh)
