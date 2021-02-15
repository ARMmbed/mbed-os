# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F103RB EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f103xb.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f103xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f103xb.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f103xb.sct)
endif()

add_library(mbed-stm32f103xb INTERFACE)

target_sources(mbed-stm32f103xb
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f103xb
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f103xb ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f103xb INTERFACE mbed-stm32f1)
