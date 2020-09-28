# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-802.15.4-sl-rail OBJECT)

target_include_directories(mbed-os-802.15.4-sl-rail
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-802.15.4-sl-rail
    PRIVATE
        NanostackRfPhyEfr32.cpp
)
