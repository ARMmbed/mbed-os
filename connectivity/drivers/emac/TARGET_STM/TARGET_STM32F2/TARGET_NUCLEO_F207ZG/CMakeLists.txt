# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nucleo-f207zg INTERFACE)

target_sources(mbed-emac-nucleo-f207zg
    INTERFACE
        stm32f2_eth_init.c
)

target_link_libraries(mbed-emac-nucleo-f207zg INTERFACE mbed-emac-stm32f2)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-nucleo-f207zg)
