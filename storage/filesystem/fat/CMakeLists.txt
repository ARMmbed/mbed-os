# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-fat STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-fat
    PUBLIC
        .
        ./include
        ./include/fat
        ./ChaN
)

target_sources(mbed-storage-fat
    PRIVATE
        source/FATFileSystem.cpp

        ChaN/ff.cpp
        ChaN/ffunicode.cpp
)

target_link_libraries(mbed-storage-fat PUBLIC mbed-storage-filesystem)