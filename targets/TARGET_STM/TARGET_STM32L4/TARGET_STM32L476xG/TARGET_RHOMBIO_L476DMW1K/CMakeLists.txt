# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(RHOMBIO_L476DMW1K INTERFACE)

target_sources(RHOMBIO_L476DMW1K
    INTERFACE
        PeripheralPins.c
)

target_include_directories(RHOMBIO_L476DMW1K
    INTERFACE
        .
)

target_link_libraries(RHOMBIO_L476DMW1K INTERFACE STM32L476xG)
