# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("B_L4S5I_IOT01A" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_B_L4S5I_IOT01A)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l4s5xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l4s5xi.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l4s5xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l4s5xi.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
)
