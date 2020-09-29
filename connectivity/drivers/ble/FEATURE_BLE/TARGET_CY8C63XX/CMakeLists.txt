# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble-cy8c63xx OBJECT)

target_include_directories(mbed-os-ble-cy8c63xx
    PUBLIC
        .
        drivers
)

target_sources(mbed-os-ble-cy8c63xx
    PRIVATE
        Psoc6BLE.cpp
        drivers/IPCPipeTransportDriver.cpp
)

mbed_link_component(mbed-os-ble-cy8c63xx mbed-os-ble mbed-os)
