# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_G474RE INTERFACE)

target_sources(NUCLEO_G474RE
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_G474RE
    INTERFACE
        .
)

target_link_libraries(NUCLEO_G474RE INTERFACE STM32G474xE)
