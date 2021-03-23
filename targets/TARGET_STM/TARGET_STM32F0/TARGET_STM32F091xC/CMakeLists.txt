# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F091RC EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f091xc.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F091XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f091xc.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f091xc.sct)
endif()

add_library(mbed-stm32f091xc INTERFACE)

target_sources(mbed-stm32f091xc
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f091xc
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f091xc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f091xc INTERFACE mbed-stm32f0)
