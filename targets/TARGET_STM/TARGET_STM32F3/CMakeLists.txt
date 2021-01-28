# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F302x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F303x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F303xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F303xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F334x8 EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32F3 INTERFACE)

target_include_directories(STM32F3
    INTERFACE
        .
)

target_sources(STM32F3
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(STM32F3 INTERFACE STM STM32F3Cube_FW)
