# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F103x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F103x8)
elseif("STM32F103xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F103xB)    
endif()

target_sources(mbed-core
    INTERFACE
        analogin_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

add_subdirectory(STM32Cube_FW)

target_include_directories(mbed-core
    INTERFACE
        .
)
