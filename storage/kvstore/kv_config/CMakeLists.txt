# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-kv-config
    INTERFACE
        .
        include
        include/kv_config
)

target_sources(mbed-storage-kv-config
    INTERFACE
        source/kv_config.cpp
)

target_link_libraries(mbed-storage-kv-config
    INTERFACE
        mbed-storage-kvstore
        mbed-storage-blockdevice
        mbed-storage-tdbstore
        mbed-storage-filesystemstore
        mbed-storage-securestore
        mbed-storage-littlefs
        mbed-storage-fat
        mbed-storage-flashiap
        mbed-storage-sd
)
