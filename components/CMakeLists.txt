# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("")

add_subdirectory(storage)
add_subdirectory(lora)

target_include_directories(mbed-os
    PUBLIC
        TARGET_PSA/inc
        TARGET_PSA/services/storage/its
)
