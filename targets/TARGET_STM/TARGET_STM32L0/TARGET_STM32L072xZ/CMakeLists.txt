# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L072CZ_LRWAN1 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l072xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l072xz.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l072xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l072xz.sct)
endif()

add_library(mbed-stm32l072xz INTERFACE)

target_sources(mbed-stm32l072xz
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(mbed-stm32l072xz
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l072xz ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l072xz INTERFACE mbed-stm32l0)
