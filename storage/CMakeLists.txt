# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all storage libraries available.
add_library(mbed-os-storage INTERFACE)

add_library(mbed-os-storage-blockdevice INTERFACE)
add_library(mbed-os-storage-dataflash INTERFACE)
add_library(mbed-os-storage-flashiap INTERFACE)
add_library(mbed-os-storage-i2cee INTERFACE)
add_library(mbed-os-storage-qspif INTERFACE)
add_library(mbed-os-storage-sd INTERFACE)
add_library(mbed-os-storage-spif INTERFACE)

add_library(mbed-os-storage-filesystem INTERFACE)
add_library(mbed-os-storage-littlefs-v2 INTERFACE)
add_library(mbed-os-storage-littlefs INTERFACE)
add_library(mbed-os-storage-fat INTERFACE)

add_library(mbed-os-storage-kvstore INTERFACE)


add_subdirectory(blockdevice)
add_subdirectory(filesystem)
add_subdirectory(kvstore)
add_subdirectory(platform)

target_include_directories(mbed-os-storage
    INTERFACE
        .
)

target_compile_definitions(mbed-os-storage
    INTERFACE
        MBED_CONF_FILESYSTEM_PRESENT=1
        MBED_CONF_CMAKE_FILESYSTEM_ENABLED=1
)
