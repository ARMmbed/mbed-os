# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-lpctarget INTERFACE)

target_include_directories(mbed-emac-lpctarget
    INTERFACE
        .
)

target_sources(mbed-emac-lpctarget
    INTERFACE
        lpc17_emac.cpp
        lpc_phy_dp83848.cpp
)
