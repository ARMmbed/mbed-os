# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-f769ni INTERFACE)

target_sources(mbed-disco-f769ni
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-disco-f769ni
    INTERFACE
        .
)

target_link_libraries(mbed-disco-f769ni INTERFACE mbed-stm32f769xi)
