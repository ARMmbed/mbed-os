# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_TAPIT_MODULE_1_2 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f777xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F777xI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f777xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f777xi.sct)
endif()

add_library(mbed-stm32f777xi INTERFACE)

target_sources(mbed-stm32f777xi
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32f777xi
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32f777xi ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f777xi INTERFACE mbed-stm32f7)
