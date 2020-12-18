# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(acore)
add_subdirectory(stack)

target_include_directories(mbed-nfc
    INTERFACE
        .
)
