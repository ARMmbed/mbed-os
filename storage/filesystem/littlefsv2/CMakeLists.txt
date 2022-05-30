# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-littlefs-v2 STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-littlefs-v2
    PUBLIC
        .
        ./include
        ./include/littlefsv2
        ./littlefs
)

target_sources(mbed-storage-littlefs-v2
    PRIVATE
        source/LittleFileSystem2.cpp

        littlefs/lfs2.c
        littlefs/lfs2_util.c
)

target_link_libraries(mbed-storage-littlefs-v2
    PUBLIC
        mbed-storage-blockdevice
        mbed-storage-filesystem
)
