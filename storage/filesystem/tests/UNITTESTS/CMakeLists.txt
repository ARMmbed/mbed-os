# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-headers-filesystem INTERFACE)

target_include_directories(mbed-headers-filesystem
    INTERFACE
        ${mbed-os_SOURCE_DIR}/storage/filesystem/littlefs
        ${mbed-os_SOURCE_DIR}/storage/filesystem/littlefs/include
        ${mbed-os_SOURCE_DIR}/storage/filesystem/include
)
