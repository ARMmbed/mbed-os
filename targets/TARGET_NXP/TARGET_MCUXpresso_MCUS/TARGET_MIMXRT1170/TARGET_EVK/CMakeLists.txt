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
        flash_api.c
        clock_config.c
        mbed_overrides.c
        PeripheralPins.c
        pinmap.c
        serial_api.c
        us_ticker.c
        

        xip/evkmimxrt1170_flexspi_nor_config.c
        xip/dcd.c
)
