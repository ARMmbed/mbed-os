# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cy8c63xx INTEFACE)

target_include_directories(mbed-ble-cy8c63xx
    INTEFACE
        .
        drivers
)

target_sources(mbed-ble-cy8c63xx
    INTEFACE
        Psoc6BLE.cpp
        drivers/IPCPipeTransportDriver.cpp
)
