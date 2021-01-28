# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L073RZ EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l073xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l073xz.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l073xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l073xz.sct)
endif()

add_library(STM32L073xZ INTERFACE)

target_sources(STM32L073xZ
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(STM32L073xZ
    INTERFACE
        .
)

mbed_set_linker_script(STM32L073xZ ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L073xZ INTERFACE STM32L0)
