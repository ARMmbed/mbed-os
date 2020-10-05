# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# This is specific driver for mbed-os-storage, it is not part of mbed-os target.
# TODO CMake: Perhaps move this/these file(s)
target_include_directories(mbed-os-device_key
    INTERFACE
        ./include
        ./include/device_key
)

target_sources(mbed-os-device_key
    INTERFACE
        source/DeviceKey.cpp
        ../source/SFDP.cpp
)

target_link_libraries(mbed-os-device_key
    INTERFACE
        mbed-os-storage-kvstore
)
