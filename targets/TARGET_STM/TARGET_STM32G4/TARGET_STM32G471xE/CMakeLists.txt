# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g471xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g471xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g471xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g471xe.sct)
endif()

add_library(STM32G471xE INTERFACE)

target_sources(STM32G471xE
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32G471xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32G471xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G471xE INTERFACE STM32G4)
