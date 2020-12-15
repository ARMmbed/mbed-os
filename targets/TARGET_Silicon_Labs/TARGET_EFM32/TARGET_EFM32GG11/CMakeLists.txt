# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("EFM32GG11_STK3701" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_EFM32GG11_STK3701
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/efm32gg11.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_efm32gg11.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/efm32gg11.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_efm32gg11.S)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c

        device/system_efm32gg11b.c
        ${STARTUP_FILE}
)
