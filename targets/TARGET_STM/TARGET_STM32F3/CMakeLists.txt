# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F303x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F303x8)
endif()

add_subdirectory(device)

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
