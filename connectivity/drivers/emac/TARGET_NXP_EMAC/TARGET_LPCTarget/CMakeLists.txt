# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-emac
    INTERFACE
        .
)

target_sources(mbed-os-emac
    INTERFACE
        lpc17_emac.cpp
        lpc_phy_dp83848.cpp
)
