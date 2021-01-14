# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_F413ZH" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_DISCO_F413ZH)
    set(PERIPHERALPINS_FILE TARGET_DISCO_F413ZH/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_DISCO_F413ZH/system_clock.c)
elseif("MTS_DRAGONFLY_F413RH" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_MTS_DRAGONFLY_F413RH)
    set(PERIPHERALPINS_FILE TARGET_MTS_DRAGONFLY_F413RH/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_MTS_DRAGONFLY_F413RH/system_clock.c)

    target_sources(mbed-core INTERFACE TARGET_MTS_DRAGONFLY_F413RH/ONBOARD_TELIT_HE910.cpp)
elseif("NUCLEO_F413ZH" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core INTERFACE TARGET_NUCLEO_F413ZH)
    set(PERIPHERALPINS_FILE TARGET_NUCLEO_F413ZH/PeripheralPins.c)
    set(SYSTEM_CLOCK_FILE TARGET_NUCLEO_F413ZH/system_clock.c)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F413xH.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f413xh.sct)
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
