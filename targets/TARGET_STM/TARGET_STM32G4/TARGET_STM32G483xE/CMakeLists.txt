# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g483xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g483xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g483xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g483xe.sct)
endif()

add_library(STM32G483xE INTERFACE)

target_sources(STM32G483xE
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32G483xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32G483xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G483xE INTERFACE STM32G4)
