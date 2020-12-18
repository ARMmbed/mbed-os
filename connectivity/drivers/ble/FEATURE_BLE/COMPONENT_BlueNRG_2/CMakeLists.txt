# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-ble-bluenrg2
    INTERFACE
        BlueNrg2HCIDriver.cpp
)

target_link_libraries(mbed-ble-bluenrg2
    INTERFACE
        mbed-ble
)
