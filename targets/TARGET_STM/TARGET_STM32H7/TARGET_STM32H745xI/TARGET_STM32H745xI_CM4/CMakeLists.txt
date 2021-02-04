# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h745xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32H745xI_CM4.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h745xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32h745xI_CM4.sct)
endif()

add_library(STM32H745xI_CM4 INTERFACE)

target_sources(STM32H745xI_CM4
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32H745xI_CM4
    INTERFACE
        .
)

mbed_set_linker_script(STM32H745xI_CM4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32H745xI_CM4 INTERFACE STM32H745xI)
