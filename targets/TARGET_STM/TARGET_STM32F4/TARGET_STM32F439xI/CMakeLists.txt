# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F439ZI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_WIO_3G EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_WIO_BG96 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f439xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F439ZI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f439xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f439xx.sct)
endif()

add_library(STM32F439xI INTERFACE)

target_sources(STM32F439xI
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F439xI
    INTERFACE
        .
)

mbed_set_linker_script(STM32F439xI ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F439xI INTERFACE STM32F4)
