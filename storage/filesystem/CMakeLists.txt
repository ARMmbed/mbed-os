# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-storage-filesystem STATIC EXCLUDE_FROM_ALL)

add_subdirectory(fat)
add_subdirectory(littlefs)
add_subdirectory(littlefsv2)

target_include_directories(mbed-storage-filesystem
    PUBLIC
        .
        ./include
        ./include/filesystem
)

target_sources(mbed-storage-filesystem
    PRIVATE
        source/Dir.cpp
        source/File.cpp
        source/FileSystem.cpp
)

target_link_libraries(mbed-storage-filesystem PUBLIC mbed-storage-blockdevice)

target_compile_definitions(mbed-storage
    INTERFACE
        MBED_CONF_FILESYSTEM_PRESENT=1
)
