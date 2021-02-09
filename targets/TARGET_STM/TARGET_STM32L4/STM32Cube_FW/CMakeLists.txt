# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stm32l4cube-fw INTERFACE)

target_sources(mbed-stm32l4cube-fw
    INTERFACE
        system_stm32l4xx.c
)

target_include_directories(mbed-stm32l4cube-fw
    INTERFACE
        .
        CMSIS
)

add_subdirectory(STM32L4xx_HAL_Driver EXCLUDE_FROM_ALL)

target_link_libraries(mbed-stm32l4cube-fw INTERFACE mbed-stm32l4xx-hal-driver)
