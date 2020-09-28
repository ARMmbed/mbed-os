# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-spif OBJECT)

target_sources(mbed-os-storage-spif
    PRIVATE
        source/SPIFBlockDevice.cpp
)

target_include_directories(mbed-os-storage-spif
    PUBLIC
        include
        include/SPIF
)
