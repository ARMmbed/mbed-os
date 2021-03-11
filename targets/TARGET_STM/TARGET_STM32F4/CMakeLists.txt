# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32F401xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F401xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F401xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F401xD EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F405xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F405xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F407xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F407xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F410x8 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F410xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F411xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F411xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F412xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F412xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F413xH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F413xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F415xx EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F417xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F417xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F423xx EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F427xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F427xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F429xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F429xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F437xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F437xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F446xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F446xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F479xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F479xG EXCLUDE_FROM_ALL)

add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32f4 INTERFACE)

target_include_directories(mbed-stm32f4
    INTERFACE
        .
)

target_sources(mbed-stm32f4
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_link_libraries(mbed-stm32f4 INTERFACE mbed-stm mbed-stm32f4cube-fw)
