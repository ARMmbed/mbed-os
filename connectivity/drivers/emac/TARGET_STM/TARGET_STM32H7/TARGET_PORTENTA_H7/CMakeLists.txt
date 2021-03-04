# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-portenta-h7 INTERFACE)

target_sources(mbed-emac-portenta-h7
    INTERFACE
        stm32h7_eth_init.c
)

target_link_libraries(mbed-emac-portenta-h7 INTERFACE mbed-emac-stm32h7)
