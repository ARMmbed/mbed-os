# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-storage-securestore STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-securestore
    PUBLIC
        .
        include
        include/securestore
)

target_sources(mbed-storage-securestore
    PRIVATE
        source/SecureStore.cpp
)

target_link_libraries(mbed-storage-securestore
    PUBLIC
        mbed-storage-kvstore
        mbed-device_key
        mbed-mbedtls
)
