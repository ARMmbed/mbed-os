# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-spif INTERFACE)

target_sources(mbed-storage-spif
    INTERFACE
        source/SPIFBlockDevice.cpp
)

target_include_directories(mbed-storage-spif
    INTERFACE
        include
        include/SPIF
)
