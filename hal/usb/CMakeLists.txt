# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        include
        include/usb
)

target_sources(mbed-os
    PRIVATE
        source/mbed_usb_phy.cpp
)
