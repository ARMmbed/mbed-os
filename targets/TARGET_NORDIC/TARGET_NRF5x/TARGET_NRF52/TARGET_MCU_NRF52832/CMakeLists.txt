# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NRF52_DK" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_NRF52_DK
    )
elseif("SDT52832B" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SDT52832B
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/nRF52832.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_nrf52832.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NRF52832.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_NRF52832.S)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        config
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c

        device/cmsis_nvic.c
        device/system_nrf52.c

        ${STARTUP_FILE}
)
