# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_WB55RG EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32wb55xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32wb55xx.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32wb55xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32wb55xx.sct)
endif()

add_library(STM32WB55xx INTERFACE)

target_sources(STM32WB55xx
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32WB55xx
    INTERFACE
        .
)

mbed_set_linker_script(STM32WB55xx ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32WB55xx INTERFACE STM32WB)
