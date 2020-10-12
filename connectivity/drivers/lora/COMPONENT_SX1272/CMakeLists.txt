# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-lorawan
    INTERFACE
        .
        ./registers
)

target_sources(mbed-os-lorawan
    INTERFACE
        SX1272_LoRaRadio.cpp
)
