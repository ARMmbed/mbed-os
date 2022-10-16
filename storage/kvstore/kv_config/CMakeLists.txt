# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-kv-config STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-kv-config
    PUBLIC
        .
        include
        include/kv_config
)

target_sources(mbed-storage-kv-config
    PRIVATE
        source/kv_config.cpp
)

target_link_libraries(mbed-storage-kv-config
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-blockdevice
        mbed-storage-tdbstore
        mbed-storage-filesystemstore
        mbed-storage-securestore
        mbed-storage-littlefs
        mbed-storage-fat
)
