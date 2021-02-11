# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-adv-wise-1570 INTERFACE)

target_sources(mbed-adv-wise-1570
    INTERFACE
        ONBOARD_QUECTEL_BC95.cpp
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-adv-wise-1570
    INTERFACE
        .
)

target_link_libraries(mbed-adv-wise-1570 INTERFACE mbed-stm32l486xg)
