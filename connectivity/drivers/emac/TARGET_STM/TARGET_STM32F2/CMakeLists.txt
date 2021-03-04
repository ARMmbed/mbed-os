# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-stm32f2 INTERFACE)

add_subdirectory(TARGET_NUCLEO_F207ZG EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-stm32f2
    INTERFACE
        .
)

target_sources(mbed-emac-stm32f2
    INTERFACE
        stm32f2_eth_conf.c
)

target_link_libraries(mbed-emac-stm32f2 INTERFACE mbed-emac-stm)
