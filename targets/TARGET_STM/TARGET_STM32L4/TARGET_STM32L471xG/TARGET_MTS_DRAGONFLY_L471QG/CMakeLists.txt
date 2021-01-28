# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(MTS_DRAGONFLY_L471QG INTERFACE)

target_sources(MTS_DRAGONFLY_L471QG
    INTERFACE
        mtqn_low_power.c
        ONBOARD_SARA4_PPP.cpp
        ublox_low_level_api.c
        PeripheralPins.c
)

target_include_directories(MTS_DRAGONFLY_L471QG
    INTERFACE
        .
)

target_link_libraries(MTS_DRAGONFLY_L471QG INTERFACE STM32L471xG)
