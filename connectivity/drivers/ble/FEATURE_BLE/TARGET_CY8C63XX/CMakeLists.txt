# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cy8c63xx INTERFACE)

target_include_directories(mbed-os-ble-cy8c63xx
    INTERFACE
        .
        drivers
)

target_sources(mbed-os-ble-cy8c63xx
    INTERFACE
        Psoc6BLE.cpp
        drivers/IPCPipeTransportDriver.cpp
)

target_link_libraries(mbed-os-ble-cy8c63xx INTERFACE mbed-os-ble)
