# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_NRF52840.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/NRF52840.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_nrf52840.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_ARM_STD/nRF52840.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE TOOLCHAIN_IAR/startup_NRF52840_IAR.S)
    set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_IAR/nRF52840.icf)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE}
        cmsis_nvic.c
        system_nrf52840.c
)
