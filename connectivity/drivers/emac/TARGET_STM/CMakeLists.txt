# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-stm INTERFACE)

add_subdirectory(TARGET_STM32F2 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F7 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H7 EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-stm
    INTERFACE
        .
)

target_sources(mbed-emac-stm
    INTERFACE
        stm32xx_emac.cpp
)
