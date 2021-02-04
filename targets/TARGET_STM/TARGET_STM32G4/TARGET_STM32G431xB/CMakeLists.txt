# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("NUCLEO_G431KB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_G431KB)
endif()

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g431xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g431xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g431xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g431xb.sct)
endif()

add_library(STM32G431xB INTERFACE)

target_sources(STM32G431xB
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(STM32G431xB
    INTERFACE
        .
)

mbed_set_linker_script(STM32G431xB ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G431xB INTERFACE STM32G4)
