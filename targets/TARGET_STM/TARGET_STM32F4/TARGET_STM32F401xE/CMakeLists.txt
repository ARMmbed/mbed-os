# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F401RE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F401RE)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f401xe.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/STM32F401XE.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f401xe.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_ARM/stm32f401xe.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE TOOLCHAIN_IAR/startup_stm32f401xe.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_IAR/stm32f401xe.icf)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core 
    INTERFACE
        ${STARTUP_FILE}
)
