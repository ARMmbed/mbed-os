# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-lorawan
    INTERFACE
        LoRaWANTimer.cpp
)

target_include_directories(mbed-lorawan
    INTERFACE
        .
)
