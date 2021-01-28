# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_H747I EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_PORTENTA_H7 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H747xI_CM7 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H747xI_CM4 EXCLUDE_FROM_ALL)

add_library(STM32H747xI INTERFACE)

target_sources(STM32H747xI
    INTERFACE
        system_clock.c
)

target_link_libraries(STM32H747xI INTERFACE STM32H7)
