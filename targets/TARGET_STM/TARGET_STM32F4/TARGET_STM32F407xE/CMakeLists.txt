# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ARCH_MAX INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F407XG.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/STM32F407xx.sct)
endif()

add_library(STM32F407xE INTERFACE)

target_sources(STM32F407xE
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F407xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32F407xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F407xE INTERFACE STM32F4)
