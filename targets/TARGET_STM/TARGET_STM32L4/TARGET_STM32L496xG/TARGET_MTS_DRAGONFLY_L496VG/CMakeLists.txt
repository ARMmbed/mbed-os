# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mts-dragonfly-l496vg INTERFACE)

target_sources(mbed-mts-dragonfly-l496vg
    INTERFACE
        ONBOARD_QUECTEL_EG25.cpp
        PeripheralPins.c
        TargetInit.c
)

target_include_directories(mbed-mts-dragonfly-l496vg
    INTERFACE
        .
)

target_link_libraries(mbed-mts-dragonfly-l496vg INTERFACE mbed-stm32l496xg)
