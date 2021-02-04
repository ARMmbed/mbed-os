# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(STM32L4Cube_FW INTERFACE)

target_sources(STM32L4Cube_FW
    INTERFACE
        system_stm32l4xx.c
)

target_include_directories(STM32L4Cube_FW
    INTERFACE
        .
        CMSIS
)

add_subdirectory(STM32L4xx_HAL_Driver EXCLUDE_FROM_ALL)
target_link_libraries(STM32L4Cube_FW INTERFACE STM32L4xx_HAL_Driver)
