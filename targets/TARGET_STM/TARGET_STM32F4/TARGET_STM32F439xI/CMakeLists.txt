# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_assembly_stm32f439xi)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32f439xx.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_stm32f439xx.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(STARTUP_FILE TOOLCHAIN_IAR/startup_stm32f439xx.S)
    endif()
    target_sources(mbed-os PRIVATE ${STARTUP_FILE})
endfunction()

function(_mbed_set_linker_file)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/STM32F439ZI.ld)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_ARM_STD/stm32f439xx.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_IAR/stm32f439xx_flash.icf)
    endif()
    set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${LINKER_FILE})
endfunction()

_mbed_get_assembly_stm32f439xi()
_mbed_set_linker_file()

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
