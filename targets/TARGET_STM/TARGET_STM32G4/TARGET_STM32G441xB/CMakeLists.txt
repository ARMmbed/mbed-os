# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g441xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g441xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g441xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g441xb.sct)
endif()

add_library(STM32G441xB INTERFACE)

target_sources(STM32G441xB
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32G441xB
    INTERFACE
        .
)

mbed_set_linker_script(STM32G441xB ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G441xB INTERFACE STM32G4)
