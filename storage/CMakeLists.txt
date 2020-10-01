# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage INTERFACE)

add_subdirectory(blockdevice)
add_subdirectory(filesystem)
add_subdirectory(kvstore)
add_subdirectory(platform)

target_include_directories(mbed-os-storage
    INTERFACE
        .
)

target_compile_definitions(mbed-os-storage
    INTERFACE
        MBED_CONF_FILESYSTEM_PRESENT=1
        MBED_CONF_CMAKE_FILESYSTEM_ENABLED=1
)
