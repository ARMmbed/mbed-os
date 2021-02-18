# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_DISCO_F413ZH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MTS_DRAGONFLY_F413RH EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_F413ZH EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32f413xh.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32f413xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32f413xh.sct)
endif()

add_library(mbed-stm32f413xh INTERFACE)

target_include_directories(mbed-stm32f413xh
    INTERFACE
        .
)

target_sources(mbed-stm32f413xh
    INTERFACE
        ${STARTUP_FILE}
        system_clock.c
)

mbed_set_linker_script(mbed-stm32f413xh ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32f413xh INTERFACE mbed-stm32f4)
