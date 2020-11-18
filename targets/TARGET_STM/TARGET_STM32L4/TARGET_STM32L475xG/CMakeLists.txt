# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_L475VG_IOT01A" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_L475VG_IOT01A)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l475xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l475xg.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE TOOLCHAIN_IAR/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_IAR/stm32l475xg.icf)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)
