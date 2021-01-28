# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l552xc.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l552xc.sct)
endif()

add_library(STM32L552xC INTERFACE)

target_sources(STM32L552xC
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32L552xC
    INTERFACE
        .
)

mbed_set_linker_script(STM32L552xC ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32L552xC INTERFACE STM32L5)
