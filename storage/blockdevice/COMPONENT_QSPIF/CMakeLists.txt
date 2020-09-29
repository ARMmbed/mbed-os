# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-storage-qspif INTERFACE)

target_include_directories(mbed-os-storage-qspif
    INTERFACE
        include
        include/QSPIF
)

target_sources(mbed-os-storage-qspif
    INTERFACE
        source/QSPIFBlockDevice.cpp
)

target_link_libraries(mbed-os-storage-qspif INTERFACE mbed-os-storage)
