# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_F439ZI" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_NUCLEO_F439ZI)
    set(PERIPHERALPINS_FILE TARGET_NUCLEO_F439ZI/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_NUCLEO_F439ZI/system_clock.c)
elseif("WIO_3G" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_WIO_3G)
    set(PERIPHERALPINS_FILE TARGET_WIO_3G/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_WIO_3G/system_clock.c)
    target_sources(mbed-core INTERFACE TARGET_WIO_3G/ONBOARD_QUECTEL_UG96.cpp)
elseif("WIO_BG96" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_WIO_BG96)
    set(PERIPHERALPINS_FILE TARGET_WIO_BG96/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_WIO_BG96/system_clock.c)
    target_sources(mbed-core INTERFACE TARGET_WIO_BG96/ONBOARD_QUECTEL_BG96.cpp)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f439xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F439ZI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f439xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f439xx.sct)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_sources(mbed-core
    INTERFACE
        ${PERIPHERALPINS_FILE}
        ${STARTUP_FILE}
        ${SYSTEM_CLOCK_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
)
