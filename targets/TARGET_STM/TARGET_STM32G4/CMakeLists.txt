# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32G431xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G431xB)
elseif("STM32G441xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G441xB)
elseif("STM32G471xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G471xE)
elseif("STM32G473xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G473xE)
elseif("STM32G474xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G474xE)
elseif("STM32G483xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G483xE)
elseif("STM32G484xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32G484xE)
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
