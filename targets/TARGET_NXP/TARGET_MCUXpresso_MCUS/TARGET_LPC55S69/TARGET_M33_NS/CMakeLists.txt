# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARMC6/startup_LPC55S69_cm33_core0.S)
    set(LINKER_FILE device/TOOLCHAIN_ARMC6/LPC55S69_cm33_core0_flash.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_LPC55S69_cm33_core0.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/LPC55S69_cm33_core0_flash.ld)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
)

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
)
