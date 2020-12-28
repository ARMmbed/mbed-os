# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F207ZG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F207ZG)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        stm32f2_eth_conf.c
)
