# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L475VG_IOT01A INTERFACE)

target_sources(DISCO_L475VG_IOT01A
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L475VG_IOT01A
    INTERFACE
        .
)

target_link_libraries(DISCO_L475VG_IOT01A INTERFACE STM32L475xG)
