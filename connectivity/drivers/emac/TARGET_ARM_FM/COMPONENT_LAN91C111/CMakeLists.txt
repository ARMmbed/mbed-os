# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-lan91c111 INTERFACE)

target_include_directories(mbed-emac-lan91c111
    INTERFACE
        .
)

target_sources(mbed-emac-lan91c111
    INTERFACE
        fvp_emac.cpp
)
