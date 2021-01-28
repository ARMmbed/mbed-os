# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F072RB EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f072xb.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f072xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f072xb.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f072xb.sct)
endif()

add_library(STM32F072xB INTERFACE)

target_sources(STM32F072xB
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32F072xB
    INTERFACE
        .
)

mbed_set_linker_script(STM32F072xB ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F072xB INTERFACE STM32F0)
