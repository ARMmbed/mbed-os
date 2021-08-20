# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ARCH_MAX INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f407xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f407xe.sct)
endif()

add_library(mbed-stm32f407xe INTERFACE)

target_include_directories(mbed-stm32f407xe
    INTERFACE
        .
)

target_sources(mbed-stm32f407xe
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

mbed_set_linker_script(mbed-stm32f407xe ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f407xe INTERFACE mbed-stm32f4)
