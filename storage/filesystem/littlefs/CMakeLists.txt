# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-littlefs
    INTERFACE
        .
        ./include
        ./include/littlefs
        ./littlefs
)

target_sources(mbed-storage-littlefs
    INTERFACE
        source/LittleFileSystem.cpp

        littlefs/lfs.c
        littlefs/lfs_util.c
)

target_link_libraries(mbed-storage-littlefs
    INTERFACE
        mbed-storage-blockdevice
        mbed-storage-filesystem
)
