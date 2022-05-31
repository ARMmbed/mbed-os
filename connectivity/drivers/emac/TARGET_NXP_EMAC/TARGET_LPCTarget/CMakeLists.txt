# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        lpc17_emac.cpp
        lpc_phy_dp83848.cpp
)
