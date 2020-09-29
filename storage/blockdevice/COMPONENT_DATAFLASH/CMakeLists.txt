# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-storage-dataflash OBJECT)

target_sources(mbed-os-storage-dataflash
    PRIVATE
        source/DataFlashBlockDevice.cpp
)

target_include_directories(mbed-os-storage-dataflash
    PUBLIC
        include
        include/DataFlash
)

mbed_link_component(mbed-os-storage-dataflash mbed-os-storage mbed-os)
