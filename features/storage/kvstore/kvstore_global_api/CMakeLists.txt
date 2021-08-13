# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-kv-global-api
    INTERFACE
        .
        include
        include/kvstore_global_api
)

target_sources(mbed-storage-kv-global-api
    INTERFACE
        source/KVMap.cpp
        source/kvstore_global_api.cpp
)

target_link_libraries(mbed-storage-kv-global-api
    INTERFACE
        mbed-storage-kvstore
        mbed-storage-kv-config
        mbed-storage-blockdevice
        mbed-storage-filesystem
)
