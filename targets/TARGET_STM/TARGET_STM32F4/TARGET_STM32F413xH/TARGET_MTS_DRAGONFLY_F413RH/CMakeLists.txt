# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(MTS_DRAGONFLY_F413RH INTERFACE)

target_sources(MTS_DRAGONFLY_F413RH
    INTERFACE
        PeripheralPins.c
        system_clock.c
        ONBOARD_TELIT_HE910.cpp
)

target_include_directories(MTS_DRAGONFLY_F413RH
    INTERFACE
        .
)

target_link_libraries(MTS_DRAGONFLY_F413RH INTERFACE STM32F413xH)
