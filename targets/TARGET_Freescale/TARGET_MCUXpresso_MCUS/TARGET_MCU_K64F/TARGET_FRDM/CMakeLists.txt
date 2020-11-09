# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
        PeripheralPins.c
)

target_include_directories(mbed-core
    INTERFACE
        .
)
