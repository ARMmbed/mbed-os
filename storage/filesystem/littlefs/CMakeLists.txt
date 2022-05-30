# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-littlefs STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-littlefs
    PUBLIC
        .
        ./include
        ./include/littlefs
        ./littlefs
)

target_sources(mbed-storage-littlefs
    PRIVATE
        source/LittleFileSystem.cpp

        littlefs/lfs.c
        littlefs/lfs_util.c
)

target_link_libraries(mbed-storage-littlefs
    PUBLIC
        mbed-storage-blockdevice
        mbed-storage-filesystem
)