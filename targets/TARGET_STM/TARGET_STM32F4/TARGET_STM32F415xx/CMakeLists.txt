# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f415xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f415xx.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f415xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f415xx.sct)
endif()

add_library(mbed-stm32f415xx INTERFACE)

target_include_directories(mbed-stm32f415xx
    INTERFACE
        .
)

target_sources(mbed-stm32f415xx
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32f415xx ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f415xx INTERFACE mbed-stm32f4)
