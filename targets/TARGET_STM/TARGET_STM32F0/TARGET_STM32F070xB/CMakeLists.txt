# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F070RB EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_mbed-stm32f070xb.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/mbed-stm32f070xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_mbed-stm32f070xb.S)
    set(LINKER_FILE TOOLCHAIN_ARM/mbed-stm32f070xb.sct)
endif()

add_library(mbed-stm32f070xB INTERFACE)

target_sources(mbed-stm32f070xB
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f070xB
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f070xB ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f070xB INTERFACE mbed-stm32f0)
