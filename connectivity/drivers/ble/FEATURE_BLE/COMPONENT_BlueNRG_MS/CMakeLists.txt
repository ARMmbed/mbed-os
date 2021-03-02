# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-bluenrg_ms INTERFACE)

target_sources(mbed-ble-bluenrg_ms
    INTERFACE
        BlueNrgMsHCIDriver.cpp
)

target_link_libraries(mbed-ble-bluenrg_ms
    INTERFACE
        mbed-ble mbed-rtos
)
