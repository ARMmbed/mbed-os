# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L475VG_IOT01A EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l475xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l475xg.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
    set(STARTUP_FILE TOOLCHAIN_IAR/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_IAR/stm32l475xg.icf)
endif()

add_library(STM32L475xG INTERFACE)

target_include_directories(STM32L475xG
    INTERFACE
        .
)

target_sources(STM32L475xG
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)


mbed_set_linker_script(STM32L475xG ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L475xG INTERFACE STM32L4)
