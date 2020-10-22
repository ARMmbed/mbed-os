# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(direct_access_devicekey)
add_subdirectory(kv_config)
add_subdirectory(securestore)

target_include_directories(mbed-storage-kvstore
    INTERFACE
        .
        ./include
        ./include/kvstore
)

target_sources(mbed-storage-kvstore
    INTERFACE
        source/FileSystemStore.cpp
        source/KVMap.cpp
        source/TDBStore.cpp
        source/kvstore_global_api.cpp
)

target_link_libraries(mbed-storage-kvstore
    INTERFACE
        mbed-device_key
        mbed-storage-blockdevice
        mbed-storage-filesystem
        mbed-storage-fat
        mbed-storage-littlefs
        mbed-storage-flashiap
        mbed-storage-sd
)
