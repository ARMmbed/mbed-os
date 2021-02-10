# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F207xG EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f2 INTERFACE)

target_sources(mbed-stm32f2
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(mbed-stm32f2
    INTERFACE
        .
        ./STM32Cube_FW
        ./STM32Cube_FW/CMSIS
        ./STM32Cube_FW/STM32F2xx_HAL_Driver
        ./STM32Cube_FW/STM32F2xx_HAL_Driver/Legacy
)

target_link_libraries(mbed-stm32f2 INTERFACE mbed-stm mbed-stm32f2cube-fw)
