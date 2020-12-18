# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32L4xx_HAL_Driver)

target_sources(mbed-core
    INTERFACE
        system_stm32l4xx.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        CMSIS
)
