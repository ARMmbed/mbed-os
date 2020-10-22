# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    PUBLIC
        include
        include/usb
)

target_sources(mbed-core
    PRIVATE
        source/mbed_usb_phy.cpp
)
