# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-802.15.4-atmel-rf OBJECT)

target_include_directories(mbed-os-802.15.4-atmel-rf
    PUBLIC
        atmel-rf-driver
        source
)

target_sources(mbed-os-802.15.4-atmel-rf
    PRIVATE
        source/NanostackRfPhyAT86RF215.cpp
        source/NanostackRfPhyAtmel.cpp
        source/at24mac.cpp
)

mbed_link_component(mbed-os-802.15.4-atmel-rf mbed-os-nanostack mbed-os)
