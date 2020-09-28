# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-802.15.4-mcr20a OBJECT)

target_include_directories(mbed-os-802.15.4-mcr20a
    PUBLIC
        mcr20a-rf-driver
        source
)

target_sources(mbed-os-802.15.4-mcr20a
    PRIVATE
        source/MCR20Drv.c
        source/NanostackRfPhyMcr20a.cpp
)
