# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    PUBLIC
        include
        include/nfc
)

target_sources(mbed-nfc
    PRIVATE
        source/m24sr_driver.cpp
)
