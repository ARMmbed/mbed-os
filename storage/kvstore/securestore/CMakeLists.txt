# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-storage-securestore
    INTERFACE
        .
        include
        include/securestore
)

target_sources(mbed-storage-securestore
    INTERFACE
        source/SecureStore.cpp
)

target_link_libraries(mbed-storage-securestore
    INTERFACE
        mbed-storage-kvstore
        mbed-device_key
        mbed-mbedtls
)
