# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-storage-filesystemstore STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-storage-filesystemstore
    PUBLIC
        .
        include
        include/filesystemstore
)

target_sources(mbed-storage-filesystemstore
    PRIVATE
        source/FileSystemStore.cpp
)

target_link_libraries(mbed-storage-filesystemstore
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-filesystem
        mbed-storage-kv-config
)
