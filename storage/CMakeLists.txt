# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all storage libraries available.
add_library(mbed-storage INTERFACE)

add_library(mbed-storage-blockdevice INTERFACE)
add_library(mbed-storage-dataflash INTERFACE)
add_library(mbed-storage-flashiap INTERFACE)
add_library(mbed-storage-i2cee INTERFACE)
add_library(mbed-storage-qspif INTERFACE)
add_library(mbed-storage-sd INTERFACE)
add_library(mbed-storage-spif INTERFACE)

add_library(mbed-storage-filesystem INTERFACE)
add_library(mbed-storage-littlefs-v2 INTERFACE)
add_library(mbed-storage-littlefs INTERFACE)
add_library(mbed-storage-fat INTERFACE)

add_library(mbed-storage-kvstore INTERFACE)


add_subdirectory(blockdevice)
add_subdirectory(filesystem)
add_subdirectory(kvstore)
add_subdirectory(platform)

target_include_directories(mbed-storage
    INTERFACE
        .
)

target_compile_definitions(mbed-storage
    INTERFACE
        MBED_CONF_FILESYSTEM_PRESENT=1
)
