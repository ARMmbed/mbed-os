# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-stm32f7 INTERFACE)

add_subdirectory(TARGET_DISCO_F746NG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_DISCO_F769NI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F746ZG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F756ZG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F767ZI EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-stm32f7
    INTERFACE
        .
)

target_sources(mbed-emac-stm32f7
    INTERFACE
        stm32f7_eth_conf.c
)

target_link_libraries(mbed-emac-stm32f7 INTERFACE mbed-emac-stm)
