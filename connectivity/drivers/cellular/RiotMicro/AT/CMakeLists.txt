# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-riotmicro-at INTERFACE)

target_include_directories(mbed-cellular-riotmicro-at
    INTERFACE
        .
)

target_sources(mbed-cellular-riotmicro-at
    INTERFACE
        RM1000_AT.cpp
        RM1000_AT_CellularContext.cpp
        RM1000_AT_CellularNetwork.cpp
        RM1000_AT_CellularStack.cpp
)
