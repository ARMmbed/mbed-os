# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-device_key
    INTERFACE
        include
        include/device_key
)

target_sources(mbed-device_key
    INTERFACE
        source/DeviceKey.cpp
)

target_link_libraries(mbed-device_key
    INTERFACE
        mbed-storage-kvstore
        mbed-storage-tdbstore
        mbed-storage-kv-global-api
)
