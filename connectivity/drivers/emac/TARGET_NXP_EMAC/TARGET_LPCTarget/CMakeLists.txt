# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-lpc17xx OBJECT)

target_include_directories(mbed-os-emac-lpc17xx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-lpc17xx
    PRIVATE
        lpc17_emac.cpp
        lpc_phy_dp83848.cpp
)
