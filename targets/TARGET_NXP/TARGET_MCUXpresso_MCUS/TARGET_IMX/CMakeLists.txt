# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-imx INTERFACE)

target_include_directories(mbed-imx
    INTERFACE
        .
)

target_sources(mbed-imx
    INTERFACE
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        spi_api.c
        watchdog_api.c
)
