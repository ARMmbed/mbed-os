# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        sl_emac.cpp
        sl_eth_hw.c
)
