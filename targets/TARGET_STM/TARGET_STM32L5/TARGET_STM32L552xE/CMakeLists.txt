# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L552ZE_Q EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l552xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l552xe.sct)
endif()

add_library(mbed-stm32l552xe INTERFACE)

target_sources(mbed-stm32l552xe
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l552xe
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l552xe ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l552xe INTERFACE mbed-stm32l5)
