# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-littlefs-v2
    INTERFACE
        .
        ./include
        ./include/littlefsv2
        ./littlefs
)

target_sources(mbed-storage-littlefs-v2
    INTERFACE
        source/LittleFileSystem2.cpp

        littlefs/lfs2.c
        littlefs/lfs2_util.c
)

target_link_libraries(mbed-storage-littlefs-v2
    INTERFACE
        mbed-storage-blockdevice
        mbed-storage-filesystem
)
