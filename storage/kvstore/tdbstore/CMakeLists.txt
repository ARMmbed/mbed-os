# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-storage-tdbstore STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-tdbstore
    PUBLIC
        .
        include
        include/tdbstore
)

target_sources(mbed-storage-tdbstore
    PRIVATE
        source/TDBStore.cpp
)

target_link_libraries(mbed-storage-tdbstore
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-blockdevice
)
