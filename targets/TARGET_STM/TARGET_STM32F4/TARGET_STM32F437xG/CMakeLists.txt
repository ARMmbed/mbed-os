# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f437xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F437xx.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f437xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f437xx.sct)
endif()

add_library(STM32F437xG INTERFACE)

target_sources(STM32F437xG
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F437xG
    INTERFACE
        .
)

mbed_set_linker_script(STM32F437xG ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F437xG INTERFACE STM32F4)
