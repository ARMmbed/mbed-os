# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lpc INTERFACE)

target_include_directories(mbed-lpc
    INTERFACE
        .
)

target_sources(mbed-lpc
    INTERFACE
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_analogin_api.c
        pinmap.c
        port_api.c
        qspi_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
)
