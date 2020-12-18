# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MTS_DRAGONFLY_F411RE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MTS_DRAGONFLY_F411RE)
elseif("MTS_MDOT_F411RE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MTS_MDOT_F411RE)
elseif("STM32F401xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F401xC)
elseif("STM32F401xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F401xE)
elseif("STM32F407xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F407xE)
elseif("STM32F407xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F407xG)
elseif("STM32F410xB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F410xB)
elseif("STM32F411xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F411xE)
elseif("STM32F412xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F412xG)
elseif("STM32F413xH" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F413xH)
elseif("STM32F429xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F429xI)
elseif("STM32F437xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F437xG)
elseif("STM32F439xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F439xI)
elseif("STM32F446xE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F446xE)
elseif("STM32F469xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F469xI)
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
        ./STM32Cube_FW
        ./STM32Cube_FW/CMSIS
        ./STM32Cube_FW/STM32F4xx_HAL_Driver
        ./STM32Cube_FW/STM32F4xx_HAL_Driver/Legacy
)
