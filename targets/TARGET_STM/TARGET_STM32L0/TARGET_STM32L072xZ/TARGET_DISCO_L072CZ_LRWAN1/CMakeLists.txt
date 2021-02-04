# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(DISCO_L072CZ_LRWAN1 INTERFACE)

target_sources(DISCO_L072CZ_LRWAN1
    INTERFACE
        PeripheralPins.c
)

target_include_directories(DISCO_L072CZ_LRWAN1
    INTERFACE
        .
)

target_link_libraries(DISCO_L072CZ_LRWAN1 INTERFACE STM32L072xZ)
