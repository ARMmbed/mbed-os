# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("PSOC6" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_PSOC6)
endif()

add_subdirectory(firmware)

target_include_directories(mbed-os-ble
    INTERFACE
        .
)

target_sources(mbed-os-ble
    INTERFACE
        CyH4TransportDriver.cpp
        HCIDriver.cpp
)
