# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    INTERFACE
        .
)

target_sources(mbed-cellular
    INTERFACE
        UBLOX_N2XX.cpp
        UBLOX_N2XX_CellularContext.cpp
        UBLOX_N2XX_CellularNetwork.cpp
        UBLOX_N2XX_CellularSMS.cpp
        UBLOX_N2XX_CellularStack.cpp
)
