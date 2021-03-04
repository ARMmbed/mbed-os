# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-quectel-bc95 INTERFACE)

target_include_directories(mbed-cellular-quectel-bc95
    INTERFACE
        .
)

target_sources(mbed-cellular-quectel-bc95
    INTERFACE
        QUECTEL_BC95.cpp
        QUECTEL_BC95_CellularContext.cpp
        QUECTEL_BC95_CellularInformation.cpp
        QUECTEL_BC95_CellularNetwork.cpp
        QUECTEL_BC95_CellularStack.cpp
)
