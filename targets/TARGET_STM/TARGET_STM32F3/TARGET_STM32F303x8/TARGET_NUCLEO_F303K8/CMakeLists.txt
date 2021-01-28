# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(NUCLEO_F303K8 INTERFACE)

target_include_directories(NUCLEO_F303K8
    INTERFACE
        .
)

target_sources(NUCLEO_F303K8
    INTERFACE
        PeripheralPins.c
)

target_link_libraries(NUCLEO_F303K8 INTERFACE STM32F303x8)
