# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f401re INTERFACE)

target_include_directories(nucleo-f401re
    INTERFACE
        .
)

target_sources(nucleo-f401re
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_link_libraries(nucleo-f401re INTERFACE mbed-stm32f401xe)
