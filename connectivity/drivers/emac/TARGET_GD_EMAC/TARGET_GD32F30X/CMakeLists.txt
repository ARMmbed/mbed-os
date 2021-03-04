# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-gd32f30x INTERFACE)

target_sources(mbed-emac-gd32f30x
    INTERFACE
        gd32f3_eth_init.c
)

target_link_libraries(mbed-emac-gd32f30x INTERFACE mbed-emac-gd)
