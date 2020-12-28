# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    INTERFACE
        .
)

target_sources(mbed-cellular
    INTERFACE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)
