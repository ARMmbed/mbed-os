# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_STM32G431xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G441xB EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G471xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G473xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G474xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G483xE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32G484xE EXCLUDE_FROM_ALL)
add_subdirectory(STM32Cube_FW EXCLUDE_FROM_ALL)

add_library(STM32G4 INTERFACE)

target_sources(STM32G4
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c
)

target_include_directories(STM32G4
    INTERFACE
        .
)

target_link_libraries(STM32G4 INTERFACE STM STM32G4Cube_FW)
