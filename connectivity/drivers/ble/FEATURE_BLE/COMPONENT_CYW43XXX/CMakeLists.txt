# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cyw43xxx INTERFACE)

add_subdirectory(TARGET_PSOC6 EXCLUDE_FROM_ALL)
add_subdirectory(firmware)

target_include_directories(mbed-ble-cyw43xxx
    INTERFACE
        .
)

target_sources(mbed-ble-cyw43xxx
    INTERFACE
        CyH4TransportDriver.cpp
        HCIDriver.cpp
)
