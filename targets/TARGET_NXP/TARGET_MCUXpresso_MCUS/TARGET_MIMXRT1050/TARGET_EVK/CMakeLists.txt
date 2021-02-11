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
        PeripheralPins.c
        fsl_clock_config.c
        fsl_phy.c
        lpm.c
        mbed_overrides.c
        specific.c

        xip/evkbimxrt1050_flexspi_nor_config.c
        xip/evkbimxrt1050_sdram_ini_dcd.c
)
