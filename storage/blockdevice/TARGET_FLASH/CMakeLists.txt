# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-flashiap STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-flashiap
    PUBLIC
        include
        include/FlashIAP
)

target_sources(mbed-storage-flashiap
    PRIVATE
        source/FlashIAPBlockDevice.cpp
)

target_link_libraries(mbed-storage-flashiap PUBLIC mbed-storage-blockdevice)