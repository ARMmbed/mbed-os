# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(WIO_3G INTERFACE)

target_sources(WIO_3G
    INTERFACE
        PeripheralPins.c
        system_clock.c
        ONBOARD_QUECTEL_UG96.cpp
)

target_include_directories(WIO_3G
    INTERFACE
        .
)

target_link_libraries(WIO_3G INTERFACE STM32F439xI)
