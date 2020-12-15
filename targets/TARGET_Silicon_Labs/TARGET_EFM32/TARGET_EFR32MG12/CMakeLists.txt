# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TB_SENSE_12" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_TB_SENSE_12
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/efr32mg12p.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_efr32mg12p.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/efr32mg12p.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_efr32mg12p.S)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c

        device/system_efr32mg12p.c
        ${STARTUP_FILE}
)
