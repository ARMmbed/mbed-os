# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-storage-qspif OBJECT)

target_include_directories(mbed-os-storage-qspif
    PUBLIC
        include
        include/QSPIF
)

target_sources(mbed-os-storage-qspif
    PRIVATE
        source/QSPIFBlockDevice.cpp
)

mbed_link_component(mbed-os-storage-qspif mbed-os-storage mbed-os)
