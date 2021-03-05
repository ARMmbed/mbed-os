# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-disco-h747i INTERFACE)

target_sources(mbed-emac-disco-h747i
    INTERFACE
        stm32h7_eth_init.c
)

target_link_libraries(mbed-emac-disco-h747i INTERFACE mbed-emac-stm32h7)

target_link_libraries(mbed-netsocket PRIVATE mbed-emac-disco-h747i)
