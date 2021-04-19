# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F746xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F756xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F767xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F769xI EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f7 INTERFACE)

target_sources(mbed-stm32f7
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        i2c_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)


target_include_directories(mbed-stm32f7
    INTERFACE
        .
)

target_link_libraries(mbed-stm32f7 INTERFACE mbed-stm mbed-stm32f7cube-fw)
