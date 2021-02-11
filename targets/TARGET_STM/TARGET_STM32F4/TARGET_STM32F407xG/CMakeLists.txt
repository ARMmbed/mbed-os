# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F407XG.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_STM32F407xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/STM32F407xx.sct)
endif()

add_library(mbed-stm32f407xg INTERFACE)

target_sources(mbed-stm32f407xg
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f407xg
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f407xg ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f407xg INTERFACE mbed-stm32f4)
