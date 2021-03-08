# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F429ZI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F429ZI EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f429xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F429xI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f429xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f429xx.sct)
endif()

add_library(mbed-stm32f429xi INTERFACE)

target_sources(mbed-stm32f429xi
    INTERFACE
        system_init_pre.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f429xi
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f429xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f429xi INTERFACE mbed-stm32f4)
