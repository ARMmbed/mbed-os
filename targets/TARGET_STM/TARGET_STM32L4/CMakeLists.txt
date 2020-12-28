# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32L432xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L432xC)
elseif("STM32L433xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L433xC)
elseif("STM32L443xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L443xC)
elseif("STM32L452xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L452xE)
elseif("STM32L471xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L471xG)
elseif("STM32L475xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L475xG)
elseif("STM32L476xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L476xG)
elseif("STM32L486xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L486xG)
elseif("STM32L496xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L496xG)
elseif("STM32L4R5xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4R5xI)
elseif("STM32L4R9xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4R9xI)
elseif("STM32L4S5xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4S5xI)
endif()

add_subdirectory(STM32Cube_FW)

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

target_include_directories(mbed-core
    INTERFACE
        .
)
