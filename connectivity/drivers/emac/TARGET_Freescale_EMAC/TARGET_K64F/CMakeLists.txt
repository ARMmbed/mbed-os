# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-k64f INTERFACE)

target_sources(mbed-emac-k64f
    INTERFACE
        hardware_init_MK64F12.c
)

target_link_libraries(mbed-emac-k64f INTERFACE mbed-emac-freescale)
