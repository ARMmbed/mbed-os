# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-ospif INTERFACE)

target_include_directories(mbed-storage-ospif
    INTERFACE
        include
        include/OSPIF
)

target_sources(mbed-storage-ospif
    INTERFACE
        source/OSPIFBlockDevice.cpp
)
