# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(fat)
add_subdirectory(littlefs)
add_subdirectory(littlefsv2)

target_include_directories(mbed-os-storage
    INTERFACE
        .
        ./include
        ./include/filesystem
)

target_sources(mbed-os-storage
    INTERFACE
        source/Dir.cpp
        source/File.cpp
        source/FileSystem.cpp
)
