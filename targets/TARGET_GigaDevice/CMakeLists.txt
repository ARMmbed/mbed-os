# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_GD32F30X EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_GD32F4XX EXCLUDE_FROM_ALL)

add_library(mbed-gigadevice INTERFACE)

target_include_directories(mbed-gigadevice
    INTERFACE
        .
)

target_link_libraries(mbed-gigadevice INTERFACE mbed-cmsis-cortex-m)
