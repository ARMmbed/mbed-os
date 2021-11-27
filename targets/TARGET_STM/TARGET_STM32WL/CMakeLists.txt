# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32WL54xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WL55xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE4x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE4xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE4xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE5x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE5xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WLE5xC EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32wl INTERFACE)

target_include_directories(mbed-stm32wl
    INTERFACE
        .
)

target_sources(mbed-stm32wl
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        i2c_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
        system_clock.c
)


target_link_libraries(mbed-stm32wl INTERFACE mbed-stm mbed-stm32wlcube-fw)
