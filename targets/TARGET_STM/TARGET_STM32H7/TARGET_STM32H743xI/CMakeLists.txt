# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_H743ZI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_H743ZI2 EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32h743xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32H743xI.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32h743xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32h743xI.sct)
endif()

add_library(STM32H743xI INTERFACE)

target_sources(STM32H743xI
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32H743xI
    INTERFACE
        .
)

mbed_set_linker_script(STM32H743xI ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32H743xI INTERFACE STM32H7)
