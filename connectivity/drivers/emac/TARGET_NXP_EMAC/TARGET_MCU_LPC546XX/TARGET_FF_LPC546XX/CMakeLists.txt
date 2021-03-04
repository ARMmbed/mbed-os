# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-ff-lpc546xx INTERFACE)

target_sources(mbed-emac-ff-lpc546xx
    INTERFACE
        hardware_init_LPC546XX.c
)

target_link_libraries(mbed-emac-ff-lpc546xx INTERFACE mbed-emac-lpc546xx)
