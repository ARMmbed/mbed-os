# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-ospif
    INTERFACE
        include
        include/OSPIF
)

target_sources(mbed-storage-ospif
    INTERFACE
        source/OSPIFBlockDevice.cpp
)
