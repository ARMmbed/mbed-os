# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_L552ZE_Q INTERFACE)

target_sources(NUCLEO_L552ZE_Q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(NUCLEO_L552ZE_Q
    INTERFACE
        .
)

target_link_libraries(NUCLEO_L552ZE_Q INTERFACE STM32L552xE)
