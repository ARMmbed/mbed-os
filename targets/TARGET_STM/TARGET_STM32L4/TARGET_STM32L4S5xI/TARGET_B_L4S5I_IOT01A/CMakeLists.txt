# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(B_L4S5I_IOT01A INTERFACE)

target_sources(B_L4S5I_IOT01A
    INTERFACE
        PeripheralPins.c
)

target_include_directories(B_L4S5I_IOT01A
    INTERFACE
        .
)

target_link_libraries(B_L4S5I_IOT01A INTERFACE STM32L4S5xI)
