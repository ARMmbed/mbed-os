# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-rx-a1xx INTERFACE)

target_include_directories(mbed-emac-rx-a1xx
    INTERFACE
        .
)

target_sources(mbed-emac-rx-a1xx
    INTERFACE
        rza1_emac.cpp
        rza1_eth.cpp
)
