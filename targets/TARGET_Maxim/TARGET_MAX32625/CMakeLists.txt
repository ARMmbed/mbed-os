# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MAX32625MBED EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MAX32625PICO EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SDT32625B EXCLUDE_FROM_ALL)
add_subdirectory(device EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MAX32625.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_max32625.S)
endif()

add_library(mbed-max32625 INTERFACE)

target_include_directories(mbed-max32625
    INTERFACE
        .
        device
        mxc
)

target_sources(mbed-max32625
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

target_link_libraries(mbed-max32625 INTERFACE mbed-maxim)
