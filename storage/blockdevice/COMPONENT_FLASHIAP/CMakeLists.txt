# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-flashiap INTERFACE)

target_include_directories(mbed-os-storage-flashiap
    INTERFACE
        include
        include/FlashIAP
)

target_sources(mbed-os-storage-flashiap
    INTERFACE
        source/FlashIAPBlockDevice.cpp
)

target_link_libraries(mbed-os-storage-flashiap INTERFACE mbed-os-storage)
