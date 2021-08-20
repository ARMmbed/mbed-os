# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l082xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l082xz.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l082xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l082xz.sct)
endif()

add_library(mbed-stm32l082xz INTERFACE)

target_sources(mbed-stm32l082xz
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l082xz
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l082xz ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l082xz INTERFACE mbed-stm32l0)
