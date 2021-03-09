# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l100xb.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l100xb.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l100xb.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l100xb.sct)
endif()

add_library(mbed-stm32l100xb INTERFACE)

target_include_directories(mbed-stm32l100xb
    INTERFACE
        .
)

target_sources(mbed-stm32l100xb
    INTERFACE
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-stm32l100xb ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l100xb INTERFACE mbed-stm32l1)
