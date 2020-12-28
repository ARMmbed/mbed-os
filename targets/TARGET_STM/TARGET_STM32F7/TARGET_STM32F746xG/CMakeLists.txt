# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_F746NG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_F746NG)
elseif("NUCLEO_F746ZG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F746ZG)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f746xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F746xG.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f746xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f746xg.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
)
