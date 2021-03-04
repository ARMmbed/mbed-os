# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cy8c63xx INTERFACE)

target_include_directories(mbed-ble-cy8c63xx
    INTERFACE
        .
        drivers
)

target_sources(mbed-ble-cy8c63xx
    INTERFACE
        Psoc6BLE.cpp
        drivers/IPCPipeTransportDriver.cpp
)

target_link_libraries(mbed-ble PRIVATE mbed-ble-cy8c63xx)
