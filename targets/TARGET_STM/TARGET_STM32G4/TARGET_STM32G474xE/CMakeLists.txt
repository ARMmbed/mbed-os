# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_G474RE EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g474xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g474xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g474xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g474xe.sct)
endif()

add_library(STM32G474xE INTERFACE)

target_sources(STM32G474xE
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32G474xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32G474xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G474xE INTERFACE STM32G4)
