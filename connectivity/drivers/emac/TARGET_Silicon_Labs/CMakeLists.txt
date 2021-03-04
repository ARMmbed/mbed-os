# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-silicon-labs INTERFACE)

target_include_directories(mbed-emac-silicon-labs
    INTERFACE
        .
)

target_sources(mbed-emac-silicon-labs
    INTERFACE
        sl_emac.cpp
        sl_eth_hw.c
)
