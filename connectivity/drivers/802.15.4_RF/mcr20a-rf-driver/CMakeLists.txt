# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        mcr20a-rf-driver
        source
)

target_sources(mbed-os
    PRIVATE
        source/MCR20Drv.c
        source/NanostackRfPhyMcr20a.cpp
)
