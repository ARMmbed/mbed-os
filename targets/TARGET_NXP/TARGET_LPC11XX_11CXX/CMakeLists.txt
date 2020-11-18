# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("LPC11XX" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_LPC11XX
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_LPC11XX/device/system_LPC11xx.c
    )

    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/TARGET_LPC11XX/startup_LPC11xx.S)
        set(LINKER_FILE device/TOOLCHAIN_ARM_STD/TARGET_LPC11XX/LPC1114.sct)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_LPC11xx.S)
        set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/TARGET_LPC11XX/LPC1114.ld)
    endif()
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        device
)

target_sources(mbed-core
    INTERFACE
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c

        device/cmsis_nvic.c
        ${STARTUP_FILE}
)
