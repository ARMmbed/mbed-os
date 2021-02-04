# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L433RC_P EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l433xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l433xc.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l433xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l433xc.sct)
endif()


add_library(STM32L433xC INTERFACE)

target_sources(STM32L433xC
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(STM32L433xC
    INTERFACE
        .
)

mbed_set_linker_script(STM32L433xC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L433xC INTERFACE STM32L4)
