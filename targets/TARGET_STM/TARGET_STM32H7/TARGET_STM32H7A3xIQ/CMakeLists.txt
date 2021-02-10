# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_H7A3ZI_Q EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h7a3xxq.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32h7a3xxq.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h7a3xxq.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32h7a3xxq.sct)
endif()

add_library(mbed-stm32h7a3xiq INTERFACE)

target_sources(mbed-stm32h7a3xiq
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32h7a3xiq
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32h7a3xiq ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32h7a3xiq INTERFACE mbed-stm32h7)
