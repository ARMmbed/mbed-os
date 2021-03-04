# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-bluenrg-2 INTERFACE)

target_sources(mbed-ble-bluenrg-2
    INTERFACE
        BlueNrg2HCIDriver.cpp
)

target_link_libraries(mbed-ble PRIVATE mbed-ble-bluenrg-2)
