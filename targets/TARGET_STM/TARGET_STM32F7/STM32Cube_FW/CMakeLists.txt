# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stm32f7cube-fw INTERFACE)

file(GLOB STM32F7xx_HAL_Driver_SOURCES STM32F7xx_HAL_Driver/*.c)
file(GLOB STM32F7xx_HAL_Driver_Legacy_SOURCES STM32F7xx_HAL_Driver/Legacy/*.c)

target_sources(mbed-stm32f7cube-fw
    INTERFACE
    system_stm32f7xx.c
    ${STM32F7xx_HAL_Driver_SOURCES}
    ${STM32F7xx_HAL_Driver_Legacy_SOURCES}
)

target_include_directories(mbed-stm32f7cube-fw
    INTERFACE
    .
    CMSIS
    STM32F7xx_HAL_Driver
    STM32F7xx_HAL_Driver/Legacy
)
