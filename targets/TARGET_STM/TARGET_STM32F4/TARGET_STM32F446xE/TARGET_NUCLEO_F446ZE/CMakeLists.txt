# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F446ZE INTERFACE)

target_sources(NUCLEO_F446ZE
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_F446ZE
    INTERFACE
        .
)

target_link_libraries(NUCLEO_F446ZE INTERFACE STM32F446xE)
