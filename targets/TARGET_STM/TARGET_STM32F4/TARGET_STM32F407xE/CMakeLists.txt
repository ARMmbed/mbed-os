# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_ARCH_MAX INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F407XG.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f407xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/STM32F407xx.sct)
endif()

add_library(mbed-stm32f407xe INTERFACE)

target_sources(mbed-stm32f407xe
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f407xe
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f407xe ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f407xe INTERFACE mbed-stm32f4)
