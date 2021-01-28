# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
add_library(STM32L4xxLegacyHALDriver INTERFACE)
target_include_directories(STM32L4xxLegacyHALDriver
    INTERFACE
        .
)

target_sources(STM32L4xxLegacyHALDriver
    INTERFACE
        stm32l4xx_hal_can_legacy.c
)
