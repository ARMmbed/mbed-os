# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c


)

add_subdirectory(STM32Cube_FW)
mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW/CMSIS
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW/STM32F4xx_HAL_Driver
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Cube_FW/STM32F4xx_HAL_Driver/Legacy
)
