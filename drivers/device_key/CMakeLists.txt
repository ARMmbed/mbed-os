# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is a specific driver for the mbed-storage CMake target.
# TODO CMake: Perhaps move this/these file(s)
target_include_directories(mbed-device_key
    INTERFACE
        ./include
        ./include/device_key
)

target_sources(mbed-device_key
    INTERFACE
        source/DeviceKey.cpp
        ../source/SFDP.cpp
)

target_link_libraries(mbed-device_key
    INTERFACE
        mbed-storage-kvstore
)
