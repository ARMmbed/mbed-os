# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F091xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F072xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F070xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F030x8 EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32F0 INTERFACE)

target_include_directories(STM32F0
    INTERFACE
        .
)

target_sources(STM32F0
    INTERFACE
        analogin_device.c
        analogout_device.c
        cmsis_nvic.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(STM32F0 INTERFACE STM STM32F0Cube_FW)
