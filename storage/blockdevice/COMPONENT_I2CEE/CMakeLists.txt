# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-storage-i2cee OBJECT)

target_sources(mbed-os-storage-i2cee
    PRIVATE
        source/I2CEEBlockDevice.cpp
)

target_include_directories(mbed-os-storage-i2cee
    PUBLIC
        include
        include/I2CEE
)

mbed_link_component(mbed-os-storage-i2cee mbed-os-storage mbed-os)
