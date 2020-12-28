# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32H743xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H743xI)
elseif("STM32H745xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H745xI)
elseif("STM32H747xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H747xI)
elseif("STM32H7A3xIQ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32H7A3xIQ)
endif()

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

add_subdirectory(STM32Cube_FW)

target_include_directories(mbed-core
    INTERFACE
        .
)
