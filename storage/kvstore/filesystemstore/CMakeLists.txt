# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT ${CMAKE_CROSSCOMPILING})
    add_subdirectory(tests/UNITTESTS)
endif()

target_include_directories(mbed-storage-filesystemstore
    INTERFACE
        .
        include
        include/filesystemstore
)

target_sources(mbed-storage-filesystemstore
    INTERFACE
        source/FileSystemStore.cpp
)

target_link_libraries(mbed-storage-filesystemstore
    INTERFACE
        mbed-storage-kvstore
        mbed-storage-filesystem
        mbed-storage-kv-config
)
