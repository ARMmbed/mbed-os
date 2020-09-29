# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-ublox-at INTERFACE)

target_include_directories(mbed-os-cellular-ublox-at
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-ublox-at
    INTERFACE
        UBLOX_AT.cpp
        UBLOX_AT_CellularContext.cpp
        UBLOX_AT_CellularNetwork.cpp
        UBLOX_AT_CellularStack.cpp
)

target_link_libraries(mbed-os-cellular-ublox-at INTERFACE mbed-os-cellular)
