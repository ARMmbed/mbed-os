# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_F411RE EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f411xe.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F411XE.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f411xe.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f411re.sct)
endif()

add_library(STM32F411xE INTERFACE)

target_sources(STM32F411xE
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F411xE
    INTERFACE
        .
)

mbed_set_linker_script(STM32F411xE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F411xE INTERFACE STM32F4)
