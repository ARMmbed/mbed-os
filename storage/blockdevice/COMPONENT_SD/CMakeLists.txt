# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-storage-sd
    INTERFACE
        source/SDBlockDevice.cpp
)

target_include_directories(mbed-storage-sd
    INTERFACE
        include
        include/SD
)
