# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F746xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F756xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F767xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F769xI EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32F7 INTERFACE)

target_sources(STM32F7
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)


target_include_directories(STM32F7
    INTERFACE
        .
)

target_link_libraries(STM32F7 INTERFACE STM STM32F7Cube_FW)
