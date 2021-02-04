# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f103xb.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f103x8.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f103xb.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f103x8.sct)
endif()

add_library(STM32F103x8 INTERFACE)

target_sources(STM32F103x8
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32F103x8
    INTERFACE
        .
)

mbed_set_linker_script(STM32F103x8 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F103x8 INTERFACE STM32F1)
