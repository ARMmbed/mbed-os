# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MAX32620FTHR" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_MAX32620FTHR
    )
elseif("SDT32620B" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SDT32620B
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MAX32620.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_max32620.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/max32620.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_max32620.S)
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
        flash_api.c
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
        device/system_max32620.c

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
        mxc/mxc_lock.c
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
