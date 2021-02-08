# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32WL55xC EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32WL INTERFACE)

target_sources(STM32WL
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(STM32WL
    INTERFACE
        .
)

target_link_libraries(STM32WL INTERFACE STM STM32WLCube_FW)
