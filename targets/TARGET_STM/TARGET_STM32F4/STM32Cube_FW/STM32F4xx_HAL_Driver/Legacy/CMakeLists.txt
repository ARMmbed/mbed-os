# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(STM32F4xxLegacyHAL INTERFACE)

target_sources(STM32F4xxLegacyHAL
    INTERFACE
        stm32f4xx_hal_can_legacy.c
)

target_include_directories(STM32F4xxLegacyHAL
    INTERFACE
        .
)
