# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MKL46Z4.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MKL46Z4.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MKL46Z4.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MKL46Z4.S)
endif()

set_property(
    GLOBAL PROPERTY
    MBED_TARGET_LINKER_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE}
)

target_include_directories(mbed-core
    INTERFACE
        .
        device
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        flash_api.c
        gpio_irq_api.c
        mbed_overrides.c
        serial_api.c
        spi_api.c

        device/system_MKL46Z4.c
        ${STARTUP_FILE}
)
