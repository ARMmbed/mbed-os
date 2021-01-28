# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L072CZ_LRWAN1 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l072xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l072xz.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l072xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l072xz.sct)
endif()

add_library(STM32L072xZ INTERFACE)

target_sources(STM32L072xZ
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(STM32L072xZ
    INTERFACE
        .
)

mbed_set_linker_script(STM32L072xZ ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L072xZ INTERFACE STM32L0)
