# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_GR_LYCHEE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_RZ_A1H EXCLUDE_FROM_ALL)

add_library(mbed-rz-a1xx INTERFACE)

target_include_directories(mbed-rz-a1xx
    INTERFACE
        .
        common
)

target_sources(mbed-rz-a1xx
    INTERFACE
        analogin_api.c
        can_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mtu2.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        trng_api.c
        us_ticker.c
        USBPhy_RZ_A1.cpp

        common/rza_io_regrw.c
)

target_link_libraries(mbed-rz-a1xx INTERFACE mbed-renesas)
