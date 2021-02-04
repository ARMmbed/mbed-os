# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F207ZG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f207xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f207xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f207xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f207xg.sct)
endif()

add_library(STM32F207xG INTERFACE)

target_sources(STM32F207xG
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32F207xG
    INTERFACE
        .
)

mbed_set_linker_script(STM32F207xG ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F207xG INTERFACE STM32F2)
