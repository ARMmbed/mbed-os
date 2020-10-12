# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-ble-blue_nrg
    INTERFACE
        BlueNrgMsHCIDriver.cpp
)

target_link_libraries(mbed-os-ble-blue_nrg
    INTERFACE
        mbed-os-ble
)
