# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32H747xI_CM4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H747xI_CM7 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_DISCO_H747I EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_PORTENTA_H7 EXCLUDE_FROM_ALL)

add_library(mbed-stm32h747xi INTERFACE)

target_sources(mbed-stm32h747xi
    INTERFACE
        system_clock.c
)

target_link_libraries(mbed-stm32h747xi INTERFACE mbed-stm32h7)
