# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_L476VG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MTS_DRAGONFLY_L471QG)
elseif("NUCLEO_L476RG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_L476RG)
elseif("RHOMBIO_L476DMW1K" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RHOMBIO_L476DMW1K)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l476xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l476xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l476xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l476xg.sct)
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
