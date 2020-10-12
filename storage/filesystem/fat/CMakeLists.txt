# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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
