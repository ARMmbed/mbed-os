# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nucle-h743zi INTERFACE)

target_sources(mbed-emac-nucle-h743zi
    INTERFACE
        stm32h7_eth_init.c
)

target_link_libraries(mbed-emac-nucle-h743zi INTERFACE mbed-emac-stm32h7)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-nucle-h743zi)
