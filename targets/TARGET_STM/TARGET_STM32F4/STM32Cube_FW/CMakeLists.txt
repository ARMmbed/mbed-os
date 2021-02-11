# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32F4xx_HAL_Driver EXCLUDE_FROM_ALL)

add_library(mbed-stm32f4cube-fw INTERFACE)

target_sources(mbed-stm32f4cube-fw
    INTERFACE
        system_stm32f4xx.c
)

target_include_directories(mbed-stm32f4cube-fw
    INTERFACE
        .
        ./CMSIS
)

target_link_libraries(mbed-stm32f4cube-fw INTERFACE mbed-stm32f4xx-hal-driver)
