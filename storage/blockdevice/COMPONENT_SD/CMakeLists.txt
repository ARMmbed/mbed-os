# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-sd OBJECT)

target_sources(mbed-os-storage-sd
    PRIVATE
        source/SDBlockDevice.cpp
)

target_include_directories(mbed-os-storage-sd
    PUBLIC
        include
        include/SD
)

target_link_libraries(mbed-os-storage-sd mbed-os-storage mbed-os)
