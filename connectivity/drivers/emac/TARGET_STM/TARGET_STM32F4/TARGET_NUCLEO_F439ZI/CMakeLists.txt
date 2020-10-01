# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-emac
    PUBLIC
        .
)

target_sources(mbed-os-emac
    PRIVATE
        stm32f4_eth_conf.c
)
