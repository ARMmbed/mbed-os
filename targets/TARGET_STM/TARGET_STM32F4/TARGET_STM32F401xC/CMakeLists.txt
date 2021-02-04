# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f401xc.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F401XC.ld)
endif()

add_library(STM32F401xC INTERFACE)

target_sources(STM32F401xC
    INTERFACE
        system_clock.c
        PeripheralPins.c
        ${STARTUP_FILE}
)

target_include_directories(STM32F401xC
    INTERFACE
        .
)

mbed_set_linker_script(STM32F401xC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F401xC INTERFACE STM32F4)
