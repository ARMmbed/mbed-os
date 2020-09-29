# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is specific driver for mbed-os-storage, it is not part of mbed-os target.

target_include_directories(mbed-os-storage
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/device_key
)

target_sources(mbed-os-storage
    INTERFACE
        source/DeviceKey.cpp
        ../source/SFDP.cpp
)
