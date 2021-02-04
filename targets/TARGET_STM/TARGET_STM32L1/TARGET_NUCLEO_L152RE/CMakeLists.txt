# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_stm32l152xe.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/STM32L152XE.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM/startup_stm32l152xe.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM/stm32l152re.sct)
endif()

add_library(NUCLEO_L152RE INTERFACE)

target_sources(NUCLEO_L152RE
    INTERFACE
        PeripheralPins.c
        device/system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(NUCLEO_L152RE
    INTERFACE
        .
        device
)

mbed_set_linker_script(NUCLEO_L152RE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(NUCLEO_L152RE INTERFACE STM32L1)
