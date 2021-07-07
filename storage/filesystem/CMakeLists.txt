# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(BUILD_GREENTEA_TESTS)
        # add greentea test
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_subdirectory(fat)
add_subdirectory(littlefs)
add_subdirectory(littlefsv2)

target_include_directories(mbed-storage-filesystem
    INTERFACE
        .
        ./include
        ./include/filesystem
)

target_sources(mbed-storage-filesystem
    INTERFACE
        source/Dir.cpp
        source/File.cpp
        source/FileSystem.cpp
)

target_compile_definitions(mbed-storage
    INTERFACE
        MBED_CONF_FILESYSTEM_PRESENT=1
)
