# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("Apollo3" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Apollo3)
endif()

target_include_directories(mbed-ble
    INTERFACE
        hal/apollo3
)

target_sources(mbed-ble
    INTERFACE
        hal/apollo3/hci_drv_apollo3.c
)
