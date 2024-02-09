# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        ALT1250_PPP.cpp
        ALT1250_PPP_CellularContext.cpp
        ALT1250_PPP_CellularNetwork.cpp
)
