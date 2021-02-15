# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L4R9I EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l4r9xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l4r9xi.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l4r9xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l4r9xi.sct)
endif()

add_library(mbed-stm32l4r9xi INTERFACE)

target_sources(mbed-stm32l4r9xi
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l4r9xi
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l4r9xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l4r9xi INTERFACE mbed-stm32l4)
