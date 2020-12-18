# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32G030xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G030xx)
elseif("STM32G031xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G031xx)
elseif("STM32G041xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G041xx)
elseif("STM32G070xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G070xx)
elseif("STM32G071xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G071xx)
elseif("STM32G081xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G081xx)
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
