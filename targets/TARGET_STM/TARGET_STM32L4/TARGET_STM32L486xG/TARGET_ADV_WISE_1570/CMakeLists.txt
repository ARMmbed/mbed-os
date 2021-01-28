# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(ADV_WISE_1570 INTERFACE)

target_sources(ADV_WISE_1570
    INTERFACE
        ONBOARD_QUECTEL_BC95.cpp
        PeripheralPins.c
        system_clock.c
)

target_include_directories(ADV_WISE_1570
    INTERFACE
        .
)

target_link_libraries(ADV_WISE_1570 INTERFACE STM32L486xG)
