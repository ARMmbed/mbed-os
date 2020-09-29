# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-storage-flashiap OBJECT)

target_include_directories(mbed-os-storage-flashiap
    PUBLIC
        include
        include/FlashIAP
)

target_sources(mbed-os-storage-flashiap
    PRIVATE
        source/FlashIAPBlockDevice.cpp
)

mbed_link_component(mbed-os-storage-flashiap mbed-os-storage mbed-os)
