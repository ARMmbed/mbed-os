# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32L100xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L100xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L151xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L151xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L151xD EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L151xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L152xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L152xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L152xD EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L152xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L162xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L162xD EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L162xE EXCLUDE_FROM_ALL)

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32l1 INTERFACE)

target_include_directories(mbed-stm32l1
    INTERFACE
        .
)

target_sources(mbed-stm32l1
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(mbed-stm32l1 INTERFACE mbed-stm mbed-stm32l1cube-fw)
