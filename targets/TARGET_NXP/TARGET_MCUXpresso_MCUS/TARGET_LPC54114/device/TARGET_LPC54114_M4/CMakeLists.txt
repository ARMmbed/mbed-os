# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_nxp)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_LPC54114_cm4.S)
    set(LINKER_FILE TOOLCHAIN_ARM_STD/LPC54114J256_cm4.sct)
    set(LIB_POWER TOOLCHAIN_ARM_STD/libpower.ar)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_LPC54114_cm4.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/LPC54114J256_cm4_flash.ld)
    set(LIB_POWER TOOLCHAIN_GCC_ARM/libpower.a)
endif()

add_library(mbed-lpc54114-m4 INTERFACE)

target_link_libraries(mbed-lpc54114-m4
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_POWER}
)

target_include_directories(mbed-lpc54114-m4
    INTERFACE
        .
)

target_sources(mbed-lpc54114-m4
    INTERFACE
        system_LPC54114_cm4.c
        ${STARTUP_FILE}
)

mbed_set_linker_script(mbed-lpc54114-m4 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

mbed_post_build_lpc_patch_vtable("LPC54114")
