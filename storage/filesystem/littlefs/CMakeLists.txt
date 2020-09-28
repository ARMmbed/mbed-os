# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-littlefs OBJECT)

target_include_directories(mbed-os-littlefs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/littlefs
        ${CMAKE_CURRENT_SOURCE_DIR}/littlefs
)

target_sources(mbed-os-littlefs
    PRIVATE
        source/LittleFileSystem.cpp

        littlefs/lfs.c
        littlefs/lfs_util.c
)
