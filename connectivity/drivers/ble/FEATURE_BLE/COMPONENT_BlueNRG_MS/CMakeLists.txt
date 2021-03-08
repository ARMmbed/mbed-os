# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-bluenrg-ms INTERFACE)

target_sources(mbed-ble-bluenrg-ms
    INTERFACE
        BlueNrgMsHCIDriver.cpp
)

target_link_libraries(mbed-ble PRIVATE mbed-ble-bluenrg-ms mbed-rtos)
