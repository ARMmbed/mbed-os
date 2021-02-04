# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(ARCH_MAX INTERFACE)

target_sources(ARCH_MAX
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(ARCH_MAX
    INTERFACE
        .
)

target_link_libraries(ARCH_MAX INTERFACE STM32F407xE)
