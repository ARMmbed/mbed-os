# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-bluenrg INTERFACE)

target_sources(mbed-os-ble-bluenrg
    INTERFACE
        BlueNrgMsHCIDriver.cpp
)

target_link_libraries(mbed-os-ble-bluenrg INTERFACE mbed-os-ble)
