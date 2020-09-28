# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fat
        ${CMAKE_CURRENT_SOURCE_DIR}/ChaN
)

target_sources(mbed-os-storage
    PRIVATE
        source/FATFileSystem.cpp

        ChaN/ff.cpp
        ChaN/ffunicode.cpp
)
