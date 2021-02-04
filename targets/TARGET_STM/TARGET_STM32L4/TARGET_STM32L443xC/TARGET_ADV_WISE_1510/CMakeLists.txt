# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(ADV_WISE_1510 INTERFACE)

target_sources(ADV_WISE_1510
    INTERFACE
        PeripheralPins.c
)

target_include_directories(ADV_WISE_1510
    INTERFACE
        .
)

target_link_libraries(ADV_WISE_1510 INTERFACE STM32L443xC)
