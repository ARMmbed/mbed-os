# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-i2cee INTERFACE)

target_sources(mbed-os-storage-i2cee
    INTERFACE
        source/I2CEEBlockDevice.cpp
)

target_include_directories(mbed-os-storage-i2cee
    INTERFACE
        include
        include/I2CEE
)

target_link_libraries(mbed-os-storage-i2cee INTERFACE mbed-os-storage)
