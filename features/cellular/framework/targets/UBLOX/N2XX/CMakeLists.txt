# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        UBLOX_N2XX.cpp
        UBLOX_N2XX_CellularContext.cpp
        UBLOX_N2XX_CellularNetwork.cpp
        UBLOX_N2XX_CellularSMS.cpp
        UBLOX_N2XX_CellularStack.cpp
)
