# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-ospif STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-ospif
    PUBLIC
        include
        include/OSPIF
)

target_sources(mbed-storage-ospif
    PRIVATE
        source/OSPIFBlockDevice.cpp
)

target_link_libraries(mbed-storage-ospif PUBLIC mbed-storage-blockdevice)