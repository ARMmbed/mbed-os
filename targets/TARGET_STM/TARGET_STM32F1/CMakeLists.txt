# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F103x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F103xB EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32F1 INTERFACE)

target_sources(STM32F1
    INTERFACE
        analogin_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(STM32F1
    INTERFACE
        .
)

target_link_libraries(STM32F1 INTERFACE STM STM32F1Cube_FW)
