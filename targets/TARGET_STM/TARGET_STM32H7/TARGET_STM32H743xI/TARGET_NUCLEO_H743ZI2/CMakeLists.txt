# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_H743ZI2 INTERFACE)

target_sources(NUCLEO_H743ZI2
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_H743ZI2
    INTERFACE
        .
)

target_link_libraries(NUCLEO_H743ZI2 INTERFACE STM32H743xI)
