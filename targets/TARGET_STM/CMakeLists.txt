# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        USBPhy_STM32.cpp
        analogin_api.c
        analogout_api.c
        can_api.c
        gpio_api.c
        gpio_irq_api.c
        hal_tick_overrides.c
        i2c_api.c
        lp_ticker.c
        mbed_crc_api.c
        mbed_overrides.c
        pinmap.c
        port_api.c
        pwmout_api.c
        qspi_api.c
        reset_reason.c
        rtc_api.c
        serial_api.c
        sleep.c
        stm_spi_api.c
        trng_api.c
        us_ticker.c
        watchdog_api.c
)

mbed_add_cmake_directory_if_labels("TARGET")
