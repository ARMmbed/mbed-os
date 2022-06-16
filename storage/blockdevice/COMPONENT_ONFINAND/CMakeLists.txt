# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-onfinand
    INTERFACE
        include
        include/ONFINAND
)

target_sources(mbed-storage-onfinand
    INTERFACE
        source/ONFINANDBlockDevice.cpp
)
