# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FRDM EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MK22FN512xxx12.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MK22F51212.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MK22FN512xxx12.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MK22F51212.S)
endif()

add_library(mbed-mcu-k22f512 INTERFACE)

target_include_directories(mbed-mcu-k22f512
    INTERFACE
        device
)

target_sources(mbed-mcu-k22f512
    INTERFACE
        device/system_MK22F51212.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-mcu-k22f512 INTERFACE mbed-mcu-k22f mbed-frdm-mcu-k22f512)

add_library(mbed-k22f INTERFACE)

mbed_set_linker_script(mbed-k22f ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-k22f INTERFACE mbed-mcu-k22f512)
