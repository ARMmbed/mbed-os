# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F412ZG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_WIO_EMW3166 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f412zx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f412xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f412zx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f412xg.sct)
endif()

add_library(mbed-stm32f412xg INTERFACE)

target_include_directories(mbed-stm32f412xg
    INTERFACE
        .
)

target_sources(mbed-stm32f412xg
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32f412xg ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f412xg INTERFACE mbed-stm32f4)
