# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32L053x8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L053x8)
elseif("STM32L071xZ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L071xZ)
elseif("STM32L072xZ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L072xZ)
elseif("STM32L073xZ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L073xZ)
elseif("STM32L082xZ" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L082xZ)
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
