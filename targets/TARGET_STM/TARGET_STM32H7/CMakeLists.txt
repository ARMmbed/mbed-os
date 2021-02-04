# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32H743xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H745xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H747xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32H7A3xIQ EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32H7 INTERFACE)

target_sources(STM32H7
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(STM32H7
    INTERFACE
        .
)

target_link_libraries(STM32H7 INTERFACE STM STM32H7Cube_FW)
