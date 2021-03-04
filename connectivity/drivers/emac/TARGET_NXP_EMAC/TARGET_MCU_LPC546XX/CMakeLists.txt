# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-lpc546xx INTERFACE)

add_subdirectory(TARGET_FF_LPC546XX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPCXpresso EXCLUDE_FROM_ALL)

target_include_directories(mbed-emac-lpc546xx
    INTERFACE
        .
)

target_sources(mbed-emac-lpc546xx
    INTERFACE
        lpc546xx_emac.cpp
)
