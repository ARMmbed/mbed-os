# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-802.15.4-atmel-rf INTERFACE)

target_include_directories(mbed-os-802.15.4-atmel-rf
    INTERFACE
        atmel-rf-driver
        source
)

target_sources(mbed-os-802.15.4-atmel-rf
    INTERFACE
        source/NanostackRfPhyAT86RF215.cpp
        source/NanostackRfPhyAtmel.cpp
        source/at24mac.cpp
)

target_link_libraries(mbed-os-802.15.4-atmel-rf INTERFACE mbed-os-nanostack)
