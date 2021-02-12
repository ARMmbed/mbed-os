# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_GD32F30X)
add_subdirectory(TARGET_GD32F4XX)

add_library(mbed-gigadevice INTERFACE)

target_include_directories(mbed-gigadevice
    INTERFACE
        .
)
