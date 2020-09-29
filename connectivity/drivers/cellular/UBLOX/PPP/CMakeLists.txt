# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-ublox-ppp INTERFACE)

target_include_directories(mbed-os-cellular-ublox-ppp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-ublox-ppp
    INTERFACE
        UBLOX_PPP.cpp
)

target_link_libraries(mbed-os-cellular-ublox-ppp INTERFACE mbed-os-cellular)

