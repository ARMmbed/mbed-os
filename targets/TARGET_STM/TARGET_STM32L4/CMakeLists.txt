# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32L432xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L433xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L443xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L452xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L471xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L475xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L476xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L486xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L496xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L4R5xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L4R9xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L4S5xI EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32l4 INTERFACE)

target_sources(mbed-stm32l4
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

target_include_directories(mbed-stm32l4
    INTERFACE
        .
)

target_link_libraries(mbed-stm32l4 INTERFACE mbed-stm mbed-stm32l4cube-fw)
