# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage OBJECT)

add_subdirectory(blockdevice)
add_subdirectory(filesystem)
add_subdirectory(kvstore)
add_subdirectory(platform)

target_include_directories(mbed-os-storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
