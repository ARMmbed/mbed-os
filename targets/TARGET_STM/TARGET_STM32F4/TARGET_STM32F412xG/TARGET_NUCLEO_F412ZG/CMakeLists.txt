# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F412ZG INTERFACE)

target_sources(NUCLEO_F412ZG
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F412ZG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F412ZG INTERFACE STM32F412xG)
