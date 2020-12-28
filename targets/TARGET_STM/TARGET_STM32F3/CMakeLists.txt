# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F302x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F302x8)
elseif("STM32F303x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F303x8)
elseif("STM32F303xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F303xC)
elseif("STM32F303xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F303xE)
elseif("STM32F334x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F334x8)
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
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)
