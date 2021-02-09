# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_stm32l152xc.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/STM32L152XC.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM/startup_stm32l152xc.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM/stm32l152rc.sct)
endif()

add_library(mbed-mote-l152rc INTERFACE)

target_sources(mbed-mote-l152rc
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-mote-l152rc
    INTERFACE
        .
        device
)

mbed_set_linker_script(mbed-mote-l152rc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-mote-l152rc INTERFACE mbed-stm32l1)
