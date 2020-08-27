# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_assembly_mk66f18)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_MK66F18.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_MK66F18.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(STARTUP_FILE TOOLCHAIN_IAR/startup_MK66F18.S)
    endif()
    target_sources(mbed-os PRIVATE ${STARTUP_FILE})
endfunction()

function(_mbed_set_mk66f18_linker_file)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/MK66FN2M0xxx18.ld)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_ARM_STD/MK66FN2M0xxx18.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_IAR/MK66FN2M0xxx18.icf)
    endif()
    set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${LINKER_FILE})
endfunction()

_mbed_get_assembly_mk66f18()
_mbed_set_mk66f18_linker_file()

target_include_directories(mbed-os
    PUBLIC
        .
)

target_sources(mbed-os
    PRIVATE
        system_MK66F18.c
)
