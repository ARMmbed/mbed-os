# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_U575ZI_Q EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32u575xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32u575xi.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32u575xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32u575xi.sct)
endif()

add_library(mbed-stm32u575xi INTERFACE)

target_include_directories(mbed-stm32u575xi
    INTERFACE
        .
)

target_sources(mbed-stm32u575xi
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

mbed_set_linker_script(mbed-stm32u575xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32u575xi INTERFACE mbed-stm32u5)
