# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")


target_sources(mbed-os
    PRIVATE
        hash_wrappers.c
)
