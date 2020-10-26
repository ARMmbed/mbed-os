# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        ONBOARD_QUECTEL_UG96.cpp
        PeripheralPins.c
        PinNames.h
        system_clock.c
)

target_include_directories(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
