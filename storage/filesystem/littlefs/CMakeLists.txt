# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-littlefs INTERFACE)

target_include_directories(mbed-os-littlefs
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/littlefs
        ${CMAKE_CURRENT_SOURCE_DIR}/littlefs
)

target_sources(mbed-os-littlefs
    INTERFACE
        source/LittleFileSystem.cpp

        littlefs/lfs.c
        littlefs/lfs_util.c
)

target_link_libraries(mbed-os-littlefs INTERFACE mbed-os-storage)
