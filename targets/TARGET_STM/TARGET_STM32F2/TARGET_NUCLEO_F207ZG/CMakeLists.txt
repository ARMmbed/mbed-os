# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f207xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F207ZGTx_FLASH.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f207xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f207xx.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        ${STARTUP_FILE}
)

add_subdirectory(STM32Cube_FW)

target_include_directories(mbed-core
    INTERFACE
        .
        device
)
