# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-cellular
    INTERFACE
        .
)

target_sources(mbed-os-cellular
    INTERFACE
        RM1000_AT.cpp
        RM1000_AT_CellularContext.cpp
        RM1000_AT_CellularNetwork.cpp
        RM1000_AT_CellularStack.cpp
)
