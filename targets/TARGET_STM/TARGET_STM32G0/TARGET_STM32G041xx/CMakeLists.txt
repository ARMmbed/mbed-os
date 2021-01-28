# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32g041xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32g041xx.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32g041xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32g041xx.sct)
endif()

add_library(STM32G041xx INTERFACE)

target_sources(STM32G041xx
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32G041xx
    INTERFACE
        .
)

mbed_set_linker_script(STM32G041xx ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32G041xx INTERFACE STM32G0)
