# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h747xx.S)
set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32H747xI.ld)

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
)

