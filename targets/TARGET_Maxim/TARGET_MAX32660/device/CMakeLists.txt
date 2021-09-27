# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE  TOOLCHAIN_ARM_STD/MAX32660.sct)
    set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_MAX32660.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE  TOOLCHAIN_GCC_ARM/max32660.ld)
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_max32660.S)
endif()

add_library(mbed-max32660-device INTERFACE)

mbed_set_linker_script(mbed-max32660-device ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-max32660-device
    INTERFACE
        .
)

target_sources(mbed-max32660-device
    INTERFACE
        ${STARTUP_FILE}
)

target_link_libraries(mbed-max32660-device
    INTERFACE 
        mbed-max32660
)
