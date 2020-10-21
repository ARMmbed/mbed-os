# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        .
)

target_sources(mbed-os
    PRIVATE
        stm32l4xx_hal_can_legacy.c
)
