# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(STM32F4xx_HAL_Driver EXCLUDE_FROM_ALL)

add_library(STM32F4Cube_FW INTERFACE)

target_sources(STM32F4Cube_FW
    INTERFACE
        system_stm32f4xx.c
)

target_include_directories(STM32F4Cube_FW
    INTERFACE
        .
        ./CMSIS
)

target_link_libraries(STM32F4Cube_FW INTERFACE STM32F4xx_HAL_Driver)
