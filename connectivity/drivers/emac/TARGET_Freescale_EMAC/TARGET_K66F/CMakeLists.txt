# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-k66f INTERFACE)

target_sources(mbed-emac-k66f
    INTERFACE
        hardware_init_MK66F18.c
)

target_link_libraries(mbed-emac-k66f INTERFACE mbed-emac-freescale)
