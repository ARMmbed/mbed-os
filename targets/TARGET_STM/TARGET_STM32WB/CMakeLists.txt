# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32WB50xx EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xx EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32WB INTERFACE)

target_sources(STM32WB
    INTERFACE
        analogin_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
        wb_sleep.c
)

target_include_directories(STM32WB
    INTERFACE
        .
)

target_link_libraries(STM32WB INTERFACE STM STM32WBCube_FW)
