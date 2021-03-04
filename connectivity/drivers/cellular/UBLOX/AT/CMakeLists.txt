# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-ublox-at INTERFACE)

target_include_directories(mbed-cellular-ublox-at
    INTERFACE
        .
)

target_sources(mbed-cellular-ublox-at
    INTERFACE
        UBLOX_AT.cpp
        UBLOX_AT_CellularContext.cpp
        UBLOX_AT_CellularNetwork.cpp
        UBLOX_AT_CellularStack.cpp
)
