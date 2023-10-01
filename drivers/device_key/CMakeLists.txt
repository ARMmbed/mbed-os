# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    endif()
endif()

add_library(mbed-device_key STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-device_key
    PUBLIC
        include
        include/device_key
)

target_sources(mbed-device_key
    PRIVATE
        source/DeviceKey.cpp
)

target_link_libraries(mbed-device_key
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-tdbstore
        mbed-storage-kv-global-api
)
