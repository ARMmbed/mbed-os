# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all storage libraries available.
add_library(mbed-storage INTERFACE)

add_library(mbed-storage-blockdevice INTERFACE)
add_library(mbed-storage-dataflash INTERFACE)
add_library(mbed-storage-flashiap INTERFACE)
add_library(mbed-storage-i2cee INTERFACE)
add_library(mbed-storage-ospif INTERFACE)
add_library(mbed-storage-qspif INTERFACE)
add_library(mbed-storage-sd INTERFACE)
add_library(mbed-storage-spif INTERFACE)

add_library(mbed-storage-filesystem INTERFACE)
add_library(mbed-storage-littlefs-v2 INTERFACE)
add_library(mbed-storage-littlefs INTERFACE)
add_library(mbed-storage-fat INTERFACE)

add_library(mbed-storage-kvstore INTERFACE)
add_library(mbed-storage-tdbstore INTERFACE)
add_library(mbed-storage-filesystemstore INTERFACE)
add_library(mbed-storage-securestore INTERFACE)
add_library(mbed-storage-kv-config INTERFACE)
add_library(mbed-storage-direct-access-devicekey INTERFACE)
add_library(mbed-storage-kv-global-api INTERFACE)

if(${CMAKE_CROSSCOMPILING})
    # The directories below contain optional target libraries
    add_subdirectory(blockdevice EXCLUDE_FROM_ALL)
    add_subdirectory(filesystem EXCLUDE_FROM_ALL)
    add_subdirectory(kvstore EXCLUDE_FROM_ALL)
    add_subdirectory(platform EXCLUDE_FROM_ALL)
else()
    # Add these subdirectories for the Unit test
    add_subdirectory(blockdevice)
    add_subdirectory(filesystem)
    add_subdirectory(kvstore)
endif()

target_include_directories(mbed-storage
    INTERFACE
        .
)
