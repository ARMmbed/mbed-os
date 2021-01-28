# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_B_L4S5I_IOT01A EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l4s5xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l4s5xi.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l4s5xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l4s5xi.sct)
endif()

add_library(STM32L4S5xI INTERFACE)

target_sources(STM32L4S5xI
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32L4S5xI
    INTERFACE
        .
)

mbed_set_linker_script(STM32L4S5xI ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L4S5xI INTERFACE STM32L4)
