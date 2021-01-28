# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(WIO_BG96 INTERFACE)

target_sources(WIO_BG96
    INTERFACE
        PeripheralPins.c
        system_clock.c
        ONBOARD_QUECTEL_BG96.cpp
)

target_include_directories(WIO_BG96
    INTERFACE
        .
)

target_link_libraries(WIO_BG96 INTERFACE STM32F439xI)
