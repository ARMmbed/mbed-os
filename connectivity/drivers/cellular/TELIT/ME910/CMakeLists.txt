# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-telit-me910 INTERFACE)

target_include_directories(mbed-cellular-telit-me910
    INTERFACE
        .
)

target_sources(mbed-cellular-telit-me910
    INTERFACE
        TELIT_ME910.cpp
        TELIT_ME910_CellularContext.cpp
        TELIT_ME910_CellularNetwork.cpp
)
