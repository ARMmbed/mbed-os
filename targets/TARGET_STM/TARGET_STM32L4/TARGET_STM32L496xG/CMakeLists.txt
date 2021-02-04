# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L496AG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_L496ZG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l496xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l496xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l496xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l496xg.sct)
endif()

add_library(STM32L496xG INTERFACE)

target_sources(STM32L496xG
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32L496xG
    INTERFACE
        .
)

mbed_set_linker_script(STM32L496xG ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L496xG INTERFACE STM32L4)
