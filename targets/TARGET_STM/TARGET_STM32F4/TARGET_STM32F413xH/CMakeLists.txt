# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F413ZH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MTS_DRAGONFLY_F413RH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F413ZH EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/STM32F413xH.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f413xh.sct)
endif()

add_library(STM32F413xH INTERFACE)

target_sources(STM32F413xH
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(STM32F413xH
    INTERFACE
        .
)

mbed_set_linker_script(STM32F413xH ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(STM32F413xH INTERFACE STM32F4)
