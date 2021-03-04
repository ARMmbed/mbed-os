# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-flashiap INTERFACE)

target_include_directories(mbed-storage-flashiap
    INTERFACE
        include
        include/FlashIAP
)

target_sources(mbed-storage-flashiap
    INTERFACE
        source/FlashIAPBlockDevice.cpp
)
