# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F207ZG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F207ZG)
endif()

target_include_directories(mbed-emac
    PUBLIC
        .
)

target_sources(mbed-emac
    PRIVATE
        stm32f2_eth_conf.c
)
