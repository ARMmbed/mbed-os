# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g473xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g473xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g473xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g473xe.sct)
endif()

add_library(STM32G473xE INTERFACE)

target_sources(STM32G473xE
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32G473xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32G473xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G473xE INTERFACE STM32G4)
