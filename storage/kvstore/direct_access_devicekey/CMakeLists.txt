# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-direct-access-devicekey
    INTERFACE
        .
        include
        include/direct_access_devicekey
)

target_sources(mbed-storage-direct-access-devicekey
    INTERFACE
        source/DirectAccessDevicekey.cpp
)

target_link_libraries(mbed-storage-direct-access-devicekey
    INTERFACE
        mbed-storage-kvstore
        mbed-storage-kv-config
)
