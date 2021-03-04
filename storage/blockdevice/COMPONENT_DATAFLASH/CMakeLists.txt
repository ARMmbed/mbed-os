# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-dataflash INTERFACE)

target_sources(mbed-storage-dataflash
    INTERFACE
        source/DataFlashBlockDevice.cpp
)

target_include_directories(mbed-storage-dataflash
    INTERFACE
        include
        include/DataFlash
)
