# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-storage-spif
    INTERFACE
        source/SPIFBlockDevice.cpp
)

target_include_directories(mbed-os-storage-spif
    INTERFACE
        include
        include/SPIF
)
