# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stm32f4xx-legacy-hal INTERFACE)

target_sources(mbed-stm32f4xx-legacy-hal
    INTERFACE
        stm32f4xx_hal_can_legacy.c
)

target_include_directories(mbed-stm32f4xx-legacy-hal
    INTERFACE
        .
)
