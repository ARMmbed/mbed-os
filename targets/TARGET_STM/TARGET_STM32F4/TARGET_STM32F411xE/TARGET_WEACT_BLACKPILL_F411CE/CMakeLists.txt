# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-weact-blackpill-f411ce INTERFACE)

target_sources(mbed-weact-blackpill-f411ce
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-weact-blackpill-f411ce
    INTERFACE
        .
)

target_link_libraries(mbed-weact-blackpill-f411ce INTERFACE mbed-stm32f411xe)
