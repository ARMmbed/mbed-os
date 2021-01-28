# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f303xc.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F303XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f303xc.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f303xc.sct)
endif()

add_library(STM32F303xC INTERFACE)

target_sources(STM32F303xC
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F303xC
    INTERFACE
        .
)

mbed_set_linker_script(STM32F303xC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F303xC INTERFACE STM32F3)
