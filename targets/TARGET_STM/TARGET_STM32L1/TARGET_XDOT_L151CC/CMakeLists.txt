# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_stm32l151xc.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/STM32L151XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM/startup_stm32l151xc.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM/stm32l151rc.sct)
endif()

add_library(XDOT_L151CC INTERFACE)

target_sources(XDOT_L151CC
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        xdot_eeprom.c
        xdot_low_power.c
        ${STARTUP_FILE}
)

target_include_directories(XDOT_L151CC
    INTERFACE
        .
        device
)

mbed_set_linker_script(XDOT_L151CC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(XDOT_L151CC INTERFACE STM32L1)
