# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_stm32l152xc.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/STM32L152XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM/startup_stm32l152xc.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM/stm32l152rc.sct)
endif()

add_library(MOTE_L152RC INTERFACE)

target_sources(MOTE_L152RC
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(MOTE_L152RC
    INTERFACE
        .
        device
)

mbed_set_linker_script(MOTE_L152RC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(MOTE_L152RC INTERFACE STM32L1)
