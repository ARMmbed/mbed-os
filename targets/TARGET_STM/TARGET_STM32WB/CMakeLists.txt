# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32WB10xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB15xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB30xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB35xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB35xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB50xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xY EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB5MxG EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(mbed-stm32wb INTERFACE)

target_include_directories(mbed-stm32wb
    INTERFACE
        .
)

target_sources(mbed-stm32wb
    INTERFACE
        analogin_device.c
        flash_api.c
        gpio_irq_device.c
        i2c_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
        wb_sleep.c
)

target_link_libraries(mbed-stm32wb INTERFACE mbed-stm mbed-stm32wbcube-fw)
