# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32L552xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L552xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L562xE EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32L5 INTERFACE)

target_sources(STM32L5
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        system_clock.c
        spi_api.c
)

target_include_directories(STM32L5
    INTERFACE
        .
)

target_link_libraries(STM32L5 INTERFACE STM STM32L5Cube_FW)
