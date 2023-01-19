# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-blackpill-f411ce INTERFACE)

target_sources(mbed-blackpill-f411ce
    INTERFACE
        system_clock_override.c
        PeripheralPins.c
)

target_include_directories(mbed-blackpill-f411ce
    INTERFACE
        .
)

target_link_libraries(mbed-blackpill-f411ce INTERFACE mbed-stm32f411xe)
