# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(WIO_EMW3166 INTERFACE)

target_sources(WIO_EMW3166
    INTERFACE
        PeripheralPins.c
)

target_include_directories(WIO_EMW3166
    INTERFACE
        .
)

target_link_libraries(WIO_EMW3166 INTERFACE STM32F412xG)
