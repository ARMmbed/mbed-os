# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mts-dragonfly-l471qg INTERFACE)

target_sources(mbed-mts-dragonfly-l471qg
    INTERFACE
        mtqn_low_power.c
        ONBOARD_SARA4_PPP.cpp
        ublox_low_level_api.c
        PeripheralPins.c
)

target_include_directories(mbed-mts-dragonfly-l471qg
    INTERFACE
        .
)

target_link_libraries(mbed-mts-dragonfly-l471qg INTERFACE mbed-stm32l471xg)
