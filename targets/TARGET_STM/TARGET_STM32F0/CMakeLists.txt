# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F091xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F091xC)
elseif("STM32F072xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F072xB)
elseif("STM32F070xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F070xB)
elseif("STM32F030x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F030x8)
endif()

add_subdirectory(STM32Cube_FW)

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        analogin_device.c
        analogout_device.c
        cmsis_nvic.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)
