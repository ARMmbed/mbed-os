# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-spif STATIC EXCLUDE_FROM_ALL)

target_sources(mbed-storage-spif
    PRIVATE
        source/SPIFBlockDevice.cpp
)

target_include_directories(mbed-storage-spif
    PUBLIC
        include
        include/SPIF
)

target_link_libraries(mbed-storage-spif PUBLIC mbed-storage-blockdevice)