# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-kvstore
    INTERFACE
        .
        ./include
        ./include/direct_access_devicekey
)

target_sources(mbed-storage-kvstore
    INTERFACE
        source/DirectAccessDevicekey.cpp
)
