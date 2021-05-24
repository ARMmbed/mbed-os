# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32wb30xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32wb30xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32wb30xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32wb30xe.sct)
endif()

add_library(mbed-stm32wb30xe INTERFACE)

target_include_directories(mbed-stm32wb30xe
    INTERFACE
        .
)

target_sources(mbed-stm32wb30xe
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32wb30xe ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32wb30xe INTERFACE mbed-stm32wb)
