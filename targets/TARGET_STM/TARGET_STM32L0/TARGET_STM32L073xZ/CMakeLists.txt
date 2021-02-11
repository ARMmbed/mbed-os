# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L073RZ EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l073xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32L073XZ.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l073xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l073xz.sct)
endif()

add_library(mbed-stm32l073xz INTERFACE)

target_sources(mbed-stm32l073xz
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

target_include_directories(mbed-stm32l073xz
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l073xz ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l073xz INTERFACE mbed-stm32l0)
