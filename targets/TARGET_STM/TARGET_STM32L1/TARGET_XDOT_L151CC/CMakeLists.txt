# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_stm32l151xc.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/STM32L151XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM/startup_stm32l151xc.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM/stm32l151rc.sct)
endif()

add_library(mbed-xdot-l151cc INTERFACE)

target_sources(mbed-xdot-l151cc
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        xdot_eeprom.c
        xdot_low_power.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-xdot-l151cc
    INTERFACE
        .
        device
)

mbed_set_linker_script(mbed-xdot-l151cc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-xdot-l151cc INTERFACE mbed-stm32l1)


# Same target as XDOT_L151CC
add_library(mbed-ff1705-l151cc INTERFACE)

target_link_libraries(mbed-ff1705-l151cc INTERFACE mbed-xdot-l151cc)

