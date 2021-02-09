# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f411re INTERFACE)

target_sources(nucleo-f411re
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f411re
    INTERFACE
        .
)

target_link_libraries(nucleo-f411re INTERFACE mbed-stm32f411xe)
