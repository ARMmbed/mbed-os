# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-802.15.4-rf
    INTERFACE
        mcr20a-rf-driver
        source
)

target_sources(mbed-802.15.4-rf
    INTERFACE
        source/MCR20Drv.c
        source/NanostackRfPhyMcr20a.cpp
)
