# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_CORDIO_LL EXCLUDE_FROM_ALL)
add_subdirectory(cordio_stack)

target_include_directories(mbed-ble-cordio
    INTERFACE
        .
)
