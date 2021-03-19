# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_WB5MMG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32wb5mxx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32wb5mxx.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32wb5mxx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32wb5mxx.sct)
endif()

add_library(mbed-stm32wb5mxx INTERFACE)

target_sources(mbed-stm32wb5mxx
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(mbed-stm32wb5mxx
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32wb5mxx ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32wb5mxx INTERFACE mbed-stm32wb)
