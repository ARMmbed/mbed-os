# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-storage-sd
    INTERFACE
        source/SDBlockDevice.cpp
)

target_include_directories(mbed-os-storage-sd
    INTERFACE
        include
        include/SD
)
