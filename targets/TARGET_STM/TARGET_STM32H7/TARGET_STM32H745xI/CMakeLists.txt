# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32H745xI_CM4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H745xI_CM7 EXCLUDE_FROM_ALL)

add_library(STM32H745xI INTERFACE)

target_sources(STM32H745xI
    INTERFACE
        system_clock.c
)

target_link_libraries(STM32H745xI INTERFACE STM32H7)
