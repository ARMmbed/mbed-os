# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32WB50xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32WB50xx)
elseif("STM32WB55xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32WB55xx)
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
