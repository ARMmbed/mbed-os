# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-evk INTERFACE)

target_include_directories(mbed-evk
    INTERFACE
        .
        xip
)

target_sources(mbed-evk
    INTERFACE
        analogin_api.c
        flash_api.c
        fsl_clock_config.c
        fsl_phy.c
        lpm.c
        mbed_overrides.c
        PeripheralPins.c
        pinmap.c
        serial_api.c
        sleep.c
        specific.c
        trng_api.c
        us_ticker.c

        xip/evkbimxrt1050_flexspi_nor_config.c
        xip/evkbimxrt1050_sdram_ini_dcd.c
)
