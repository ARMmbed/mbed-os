# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F746xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F746xG)
elseif("STM32F756xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F756xG)
elseif("STM32F767xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F767xI)
elseif("STM32F769xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F769xI)
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
