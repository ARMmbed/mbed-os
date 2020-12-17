# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if ("GR_LYCHEE" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_GR_LYCHEE)
elseif("RZ_A1H" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_RZ_A1H)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
        common
)

target_sources(mbed-core
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

        common/rza_io_regrw.c
)
