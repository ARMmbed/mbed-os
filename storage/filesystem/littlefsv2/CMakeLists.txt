# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-littlefs-v2 INTERFACE)

target_include_directories(mbed-os-storage-littlefs-v2
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/littlefsv2
        ${CMAKE_CURRENT_SOURCE_DIR}/littlefs
)

target_sources(mbed-os-storage-littlefs-v2
    INTERFACE
        source/LittleFileSystem2.cpp

        littlefs/lfs2.c
        littlefs/lfs2_util.c
)

target_link_libraries(mbed-os-littlefs-v2 INTERFACE mbed-os-storage)
