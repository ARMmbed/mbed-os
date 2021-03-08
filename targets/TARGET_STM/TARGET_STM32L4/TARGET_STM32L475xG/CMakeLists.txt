# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_L475VG_IOT01A EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l475xg.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l475xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l475xg.sct)
endif()

add_library(mbed-stm32l475xg INTERFACE)

target_include_directories(mbed-stm32l475xg
    INTERFACE
        .
)

target_sources(mbed-stm32l475xg
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)


mbed_set_linker_script(mbed-stm32l475xg ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l475xg INTERFACE mbed-stm32l4)
