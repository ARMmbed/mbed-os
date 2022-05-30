# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-direct-access-devicekey STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-direct-access-devicekey
    PUBLIC
        .
        include
        include/direct_access_devicekey
)

target_sources(mbed-storage-direct-access-devicekey
    PRIVATE
        source/DirectAccessDevicekey.cpp
)

target_link_libraries(mbed-storage-direct-access-devicekey
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-kv-config
)
