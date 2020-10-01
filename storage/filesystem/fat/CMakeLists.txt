# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-fat INTERFACE)

target_include_directories(mbed-os-storage-fat
    INTERFACE
        .
        ./include
        ./include/fat
        ./ChaN
)

target_sources(mbed-os-storage-fat
    INTERFACE
        source/FATFileSystem.cpp

        ChaN/ff.cpp
        ChaN/ffunicode.cpp
)

target_link_libraries(mbed-os-storage-fat
    INTERFACE
        mbed-os-storage
)

