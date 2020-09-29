# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-bluenrg OBJECT)

target_sources(mbed-os-ble-bluenrg
    PRIVATE
        BlueNrgMsHCIDriver.cpp
)

target_link_libraries(mbed-os-ble-bluenrg mbed-os-ble mbed-os)
