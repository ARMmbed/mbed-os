# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MAX32625MBED" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MAX32625MBED
    )
elseif("MAX32625PICO" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MAX32625PICO
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_MAX32625PICO/low_level_init.c
    )
elseif("SDT32625B" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SDT32625B
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    if("MAX32625_BOOT" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE device/TOOLCHAIN_ARM_STD/TARGET_MAX32625_BOOT/MAX32625.sct)
    elseif("MAX32625_NO_BOOT" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE device/TOOLCHAIN_ARM_STD/TARGET_MAX32625_NO_BOOT/MAX32625.sct)
    endif()

    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MAX32625.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    if("MAX32625_BOOT" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/TARGET_MAX32625_BOOT/max32625.ld)
    elseif("MAX32625_NO_BOOT" IN_LIST MBED_TARGET_LABELS)
        set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/TARGET_MAX32625_NO_BOOT/max32625.ld)
    endif()

    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_max32625.S)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        .
        device
        mxc
)

target_sources(mbed-core
    INTERFACE
        PeripheralPins.c
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c

        device/device_nvic.c
        device/system_max32625.c

        mxc/adc.c
        mxc/clkman.c
        mxc/crc.c
        mxc/flc.c
        mxc/gpio.c
        mxc/i2cm.c
        mxc/i2cs.c
        mxc/icc.c
        mxc/ioman.c
        mxc/lp.c
        mxc/maa.c
        mxc/mxc_aes.c
        mxc/mxc_assert.c
        mxc/mxc_sys.c
        mxc/owm.c
        mxc/pmu.c
        mxc/prng.c
        mxc/pt.c
        mxc/rtc.c
        mxc/spim.c
        mxc/spis.c
        mxc/spix.c
        mxc/tmr.c
        mxc/tmr_utils.c
        mxc/uart.c
        mxc/wdt.c
        mxc/wdt2.c

        ${STARTUP_FILE}
)
