# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l082xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l082xz.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l082xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l082xz.sct)
endif()

add_library(STM32L082xZ INTERFACE)

target_sources(STM32L082xZ
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32L082xZ
    INTERFACE
        .
)

mbed_set_linker_script(STM32L082xZ ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L082xZ INTERFACE STM32L0)
