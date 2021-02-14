# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-efm32mg12 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/efr32mg12p.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_efr32mg12p.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/efr32mg12p.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_efr32mg12p.S)
endif()

target_include_directories(mbed-efm32mg12
    INTERFACE
        device
)

target_sources(mbed-efm32mg12
    INTERFACE
        PeripheralPins.c

        device/system_efr32mg12p.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-efm32mg12 INTERFACE mbed-efm32)

mbed_set_linker_script(mbed-efm32mg12 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

add_library(mbed-tb-sense-12 INTERFACE)

target_include_directories(mbed-tb-sense-12
    INTERFACE
        TARGET_TB_SENSE_12
)

target_link_libraries(mbed-tb-sense-12 INTERFACE mbed-efm32mg12 mbed-sl-rail-efr32-12)
