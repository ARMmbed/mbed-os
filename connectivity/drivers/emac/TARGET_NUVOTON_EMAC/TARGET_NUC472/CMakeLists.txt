# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        nuc472_eth.c
)
