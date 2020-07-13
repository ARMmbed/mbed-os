# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_assembly_nrf52840_dk)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_NRF52840.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE TOOLCHAIN_ARM/startup_nrf52840.S)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(STARTUP_FILE TOOLCHAIN_IAR/startup_NRF52840_IAR.S)
    endif()
    target_sources(mbed-os PRIVATE ${STARTUP_FILE})
endfunction()

function(_mbed_set_linker_file)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_GCC_ARM/NRF52840.ld)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_ARM/nRF52840.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "IAR")
        set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/TOOLCHAIN_IAR/nRF52840.icf)
    endif()
    set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${LINKER_FILE})
endfunction()

_mbed_get_assembly_nrf52840_dk()
_mbed_set_linker_file()

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        cmsis_nvic.c
        system_nrf52840.c
)
