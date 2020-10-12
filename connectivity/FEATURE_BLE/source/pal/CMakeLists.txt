# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-ble
    INTERFACE
        .
)

target_sources(mbed-os-ble
    INTERFACE
        PalAttClientToGattClient.cpp
)
