# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-ublox-at OBJECT)

target_include_directories(mbed-os-cellular-ublox-at
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-ublox-at
    PRIVATE
        UBLOX_AT.cpp
        UBLOX_AT_CellularContext.cpp
        UBLOX_AT_CellularNetwork.cpp
        UBLOX_AT_CellularStack.cpp
)
