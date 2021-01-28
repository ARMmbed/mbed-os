# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F303ZE INTERFACE)

target_sources(NUCLEO_F303ZE
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F303ZE
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F303ZE INTERFACE STM32F303xE)
