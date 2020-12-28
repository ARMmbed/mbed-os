# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("EFM32GG_STK3700" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_EFM32GG_STK3700
    )
endif()

if("1024K" IN_LIST MBED_TARGET_LABELS)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE device/TARGET_1024K/TOOLCHAIN_ARM_STD/efm32gg.sct)
        set(STARTUP_FILE device/TARGET_1024K/TOOLCHAIN_ARM_STD/startup_efm32gg.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE device/TARGET_1024K/TOOLCHAIN_GCC_ARM/efm32gg.ld)
        set(STARTUP_FILE device/TARGET_1024K/TOOLCHAIN_GCC_ARM/startup_efm32gg.S)
    endif()
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c

        device/system_efm32gg.c
        ${STARTUP_FILE}
)
