# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/littlefsv2
        ${CMAKE_CURRENT_SOURCE_DIR}/littlefs
)

target_sources(mbed-os
    PRIVATE
        source/LittleFileSystem2.cpp

        littlefs/lfs2.c
        littlefs/lfs2_util.c
)
