# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-i2cee INTERFACE)

target_sources(mbed-storage-i2cee
    INTERFACE
        source/I2CEEBlockDevice.cpp
)

target_include_directories(mbed-storage-i2cee
    INTERFACE
        include
        include/I2CEE
)
