# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F303RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F303ZE EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f303xe.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F303XE.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f303xe.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f303xe.sct)
endif()

add_library(STM32F303xE INTERFACE)

target_sources(STM32F303xE
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32F303xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32F303xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F303xE INTERFACE STM32F3)
