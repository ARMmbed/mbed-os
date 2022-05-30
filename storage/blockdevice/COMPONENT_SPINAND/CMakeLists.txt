# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-spinand STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-spinand
    PUBLIC
        include
        include/SPINAND
)

target_sources(mbed-storage-spinand
    PRIVATE
        source/SPINANDBlockDevice.cpp
)

target_link_libraries(mbed-storage-spinand PUBLIC mbed-storage-blockdevice)