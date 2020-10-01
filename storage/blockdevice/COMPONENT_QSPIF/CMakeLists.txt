# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-storage
    INTERFACE
        include
        include/QSPIF
)

target_sources(mbed-os-storage
    INTERFACE
        source/QSPIFBlockDevice.cpp
)
