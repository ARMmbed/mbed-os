# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-ambiq-micro INTERFACE)

add_subdirectory(TARGET_Apollo3)

target_include_directories(mbed-ble-ambiq-micro
    INTERFACE
        hal/apollo3
)

target_sources(mbed-ble-ambiq-micro
    INTERFACE
        hal/apollo3/hci_drv_apollo3.c
)
