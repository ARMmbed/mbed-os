# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-bluenrg_2 INTERFACE)

target_sources(mbed-ble-bluenrg_2
    INTERFACE
        BlueNrg2HCIDriver.cpp
)

target_link_libraries(mbed-ble-bluenrg_2
    INTERFACE
        mbed-ble mbed-core
)
