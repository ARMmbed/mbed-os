# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-lorawan
    INTERFACE
        LoRaWANTimer.cpp
)

target_include_directories(mbed-os-lorawan
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
