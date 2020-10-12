# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-storage-littlefs
    INTERFACE
        .
        ./include
        ./include/littlefs
        ./littlefs
)

target_sources(mbed-os-storage-littlefs
    INTERFACE
        source/LittleFileSystem.cpp

        littlefs/lfs.c
        littlefs/lfs_util.c
)

target_link_libraries(mbed-os-storage-littlefs
    INTERFACE
        mbed-os-storage-blockdevice
        mbed-os-storage-filesystem
)
