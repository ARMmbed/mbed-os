# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_WB55RG INTERFACE)

target_sources(NUCLEO_WB55RG
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_WB55RG
    INTERFACE
        .
)

target_link_libraries(NUCLEO_WB55RG INTERFACE STM32WB55xx)
