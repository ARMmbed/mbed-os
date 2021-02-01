# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    INTERFACE
        include
        include/nfc
)

target_sources(mbed-nfc
    INTERFACE
        source/m24sr_driver.cpp
)
