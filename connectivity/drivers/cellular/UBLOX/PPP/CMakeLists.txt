# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-ublox-ppp INTERFACE)

target_include_directories(mbed-cellular-ublox-ppp
    INTERFACE
        .
)

target_sources(mbed-cellular-ublox-ppp
    INTERFACE
        UBLOX_PPP.cpp
)
