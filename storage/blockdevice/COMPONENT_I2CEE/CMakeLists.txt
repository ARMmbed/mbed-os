# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-i2cee STATIC EXCLUDE_FROM_ALL)

target_sources(mbed-storage-i2cee
    PRIVATE
        source/I2CEEBlockDevice.cpp
)

target_include_directories(mbed-storage-i2cee
    PUBLIC
        include
        include/I2CEE
)

target_link_libraries(mbed-storage-i2cee PUBLIC mbed-storage-blockdevice)