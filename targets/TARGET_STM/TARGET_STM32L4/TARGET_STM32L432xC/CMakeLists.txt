# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L432KC EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l432xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l432xc.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l432xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l432xc.sct)
endif()

add_library(mbed-stm32l432xc INTERFACE)

target_sources(mbed-stm32l432xc
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(mbed-stm32l432xc
    INTERFACE
        .
)
mbed_set_linker_script(mbed-stm32l432xc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})
target_link_libraries(mbed-stm32l432xc INTERFACE mbed-stm32l4)
