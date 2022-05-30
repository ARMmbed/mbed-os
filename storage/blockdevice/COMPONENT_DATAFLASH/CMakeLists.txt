# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-dataflash STATIC EXCLUDE_FROM_ALL)

target_sources(mbed-storage-dataflash
    PRIVATE
        source/DataFlashBlockDevice.cpp
)

target_include_directories(mbed-storage-dataflash
    PUBLIC
        include
        include/DataFlash
)

target_link_libraries(mbed-storage-dataflash PUBLIC mbed-storage-blockdevice)