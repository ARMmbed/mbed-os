# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-ublox-ppp OBJECT)

target_include_directories(mbed-os-cellular-ublox-ppp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-ublox-ppp
    PRIVATE
        UBLOX_PPP.cpp
)
