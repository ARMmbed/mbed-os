# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-f746ng INTERFACE)

target_sources(mbed-disco-f746ng
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-disco-f746ng
    INTERFACE
        .
)

target_link_libraries(mbed-disco-f746ng INTERFACE mbed-stm32f746xg)
