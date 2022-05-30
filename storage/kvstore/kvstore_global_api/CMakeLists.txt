# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-kv-global-api STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-kv-global-api
    PUBLIC
        .
        include
        include/kvstore_global_api
)

target_sources(mbed-storage-kv-global-api
    PRIVATE
        source/KVMap.cpp
        source/kvstore_global_api.cpp
)

target_link_libraries(mbed-storage-kv-global-api
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-kv-config
        mbed-storage-blockdevice
        mbed-storage-filesystem
)
