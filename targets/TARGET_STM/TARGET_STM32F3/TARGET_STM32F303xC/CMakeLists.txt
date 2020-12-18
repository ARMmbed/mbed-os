# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F303K8" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F303K8)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f303xc.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F303XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f303xc.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f303xc.sct)
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
