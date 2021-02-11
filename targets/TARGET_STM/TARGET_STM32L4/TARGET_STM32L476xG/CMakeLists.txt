# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L476VG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_L476RG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RHOMBIO_L476DMW1K EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l476xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l476xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l476xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l476xg.sct)
endif()


add_library(mbed-stm32l476xg INTERFACE)

target_sources(mbed-stm32l476xg
    INTERFACE
        system_clock.c
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l476xg
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l476xg ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l476xg INTERFACE mbed-stm32l4)
