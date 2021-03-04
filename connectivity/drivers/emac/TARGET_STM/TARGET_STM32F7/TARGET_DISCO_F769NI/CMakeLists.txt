# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nucleo-f769ni INTERFACE)

target_sources(mbed-emac-nucleo-f769ni
    INTERFACE
        stm32f7_eth_init.c
)

target_link_libraries(mbed-emac-nucleo-f769ni INTERFACE mbed-emac-stm32f7)
