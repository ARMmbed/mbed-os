# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(direct_access_devicekey)
add_subdirectory(kv_config)
add_subdirectory(securestore)

target_include_directories(mbed-os-storage-kvstore
    INTERFACE
        .
        ./include
        ./include/kvstore
)

target_sources(mbed-os-storage-kvstore
    INTERFACE
        source/FileSystemStore.cpp
        source/KVMap.cpp
        source/TDBStore.cpp
        source/kvstore_global_api.cpp
)

target_link_libraries(mbed-os-storage-kvstore
    INTERFACE
        mbed-os-device_key
        mbed-os-storage-blockdevice
        mbed-os-storage-filesystem
        mbed-os-storage-fat
        mbed-os-storage-littlefs
        mbed-os-storage-flashiap
        mbed-os-storage-sd
)
