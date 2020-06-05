# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(storage)

target_include_directories(mbed-os
    PUBLIC
        TARGET_PSA/inc
        TARGET_PSA/services/storage/its
)
