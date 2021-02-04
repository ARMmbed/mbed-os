# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MTS_DRAGONFLY_F411RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MTS_MDOT_F411RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F401xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F401xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F407xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F407xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F410xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F411xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F412xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F413xH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F429xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F437xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F446xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F469xI EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32F4 INTERFACE)

target_sources(STM32F4
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(STM32F4
    INTERFACE
        .
)

target_link_libraries(STM32F4 INTERFACE STM STM32F4Cube_FW)
