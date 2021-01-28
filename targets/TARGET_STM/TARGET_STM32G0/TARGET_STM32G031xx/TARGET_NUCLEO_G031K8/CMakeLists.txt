# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_G031K8 INTERFACE)

target_sources(NUCLEO_G031K8
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(NUCLEO_G031K8
    INTERFACE
        .
)

target_link_libraries(NUCLEO_G031K8 INTERFACE STM32G031xx)
