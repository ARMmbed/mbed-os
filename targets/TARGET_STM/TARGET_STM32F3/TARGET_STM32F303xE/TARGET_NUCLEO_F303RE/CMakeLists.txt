# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F303RE INTERFACE)

target_sources(NUCLEO_F303RE
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_F303RE
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F303RE INTERFACE STM32F303xE)
