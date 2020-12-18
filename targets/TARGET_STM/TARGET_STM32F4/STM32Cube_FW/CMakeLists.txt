# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32F4xx_HAL_Driver)

target_sources(mbed-core
    INTERFACE
        system_stm32f4xx.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        ./CMSIS
)
