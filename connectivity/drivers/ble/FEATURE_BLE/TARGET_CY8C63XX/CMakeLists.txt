# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cy8c63xx OBJECT)

target_include_directories(mbed-os-cy8c63xx
    PUBLIC
        .
        drivers
)

target_sources(mbed-os-cy8c63xx
    PRIVATE
        Psoc6BLE.cpp
        drivers/IPCPipeTransportDriver.cpp
)
