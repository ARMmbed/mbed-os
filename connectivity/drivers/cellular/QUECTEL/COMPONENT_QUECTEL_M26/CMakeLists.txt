# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    PUBLIC
        .
)

target_sources(mbed-cellular
    PRIVATE
        QUECTEL_M26.cpp
        QUECTEL_M26_CellularContext.cpp
        QUECTEL_M26_CellularInformation.cpp
        QUECTEL_M26_CellularStack.cpp
)
