# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        mtqn_low_power.c
        ONBOARD_SARA4_PPP.cpp
        ublox_low_level_api.c
        PeripheralPins.c
)

target_include_directories(mbed-core
    INTERFACE
        .
)
