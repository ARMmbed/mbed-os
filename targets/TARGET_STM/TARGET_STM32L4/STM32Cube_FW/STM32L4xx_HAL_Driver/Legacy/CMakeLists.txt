# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stm32l4xx-legacy-hal-driver INTERFACE)

target_include_directories(mbed-stm32l4xx-legacy-hal-driver
    INTERFACE
        .
)

target_sources(mbed-stm32l4xx-legacy-hal-driver
    INTERFACE
        stm32l4xx_hal_can_legacy.c
)
