# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32wl54xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32wl54xc.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32wl54xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32wl54xc.sct)
endif()

add_library(mbed-stm32wl54xc INTERFACE)

target_include_directories(mbed-stm32wl54xc
    INTERFACE
        .
)

target_sources(mbed-stm32wl54xc
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32wl54xc ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32wl54xc INTERFACE mbed-stm32wl)
