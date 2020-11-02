# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NRF52840_DK" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NRF52840_DK)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_NRF52840.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/NRF52840.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_nrf52840.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/nRF52840.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE device/TOOLCHAIN_IAR/startup_NRF52840_IAR.S)
    set(LINKER_FILE device/TOOLCHAIN_IAR/nRF52840.icf)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/device
)

target_sources(mbed-core
    INTERFACE
        ${STARTUP_FILE})
        PeripheralPins.c
        USBPhy_Nordic.cpp

        device/cmsis_nvic.c
        device/system_nrf52840.c
)
