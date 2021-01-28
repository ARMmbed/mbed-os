# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F756ZG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f756xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F756xG.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f756xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f756xg.sct)
endif()

add_library(STM32F756xG INTERFACE)

target_sources(STM32F756xG
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F756xG
    INTERFACE
        .
)

mbed_set_linker_script(STM32F756xG ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F756xG INTERFACE STM32F7)
