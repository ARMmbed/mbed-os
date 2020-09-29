# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-802.15.4-mcr20a INTERFACE)

target_include_directories(mbed-os-802.15.4-mcr20a
    INTERFACE
        mcr20a-rf-driver
        source
)

target_sources(mbed-os-802.15.4-mcr20a
    INTERFACE
        source/MCR20Drv.c
        source/NanostackRfPhyMcr20a.cpp
)

target_link_libraries(mbed-os-802.15.4-mcr20a INTERFACE mbed-os-nanostack)
