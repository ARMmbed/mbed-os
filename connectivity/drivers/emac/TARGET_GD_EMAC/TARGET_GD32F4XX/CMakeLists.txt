# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-gd32f4xx INTERFACE)

target_sources(mbed-emac-gd32f4xx
    INTERFACE
        gd32f4_eth_init.c
)

target_link_libraries(mbed-emac-gd32f4xx INTERFACE mbed-emac-gd)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-gd32f4xx)
