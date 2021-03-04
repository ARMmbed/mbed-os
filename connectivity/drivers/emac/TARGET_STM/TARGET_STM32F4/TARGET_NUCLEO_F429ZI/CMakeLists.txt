# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-nucleo-f429zi INTERFACE)

target_include_directories(mbed-emac-nucleo-f429zi
    INTERFACE
        .
)

target_sources(mbed-emac-nucleo-f429zi
    INTERFACE
        stm32f4_eth_conf.c
)

target_link_libraries(mbed-emac-nucleo-f429zi INTERFACE mbed-emac-stm32f4)
