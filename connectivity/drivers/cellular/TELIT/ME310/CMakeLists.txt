# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-cellular
    INTERFACE
        .
)

target_sources(mbed-os-cellular
    INTERFACE
        TELIT_ME310.cpp
        TELIT_ME310_CellularContext.cpp
        TELIT_ME310_CellularNetwork.cpp
        TELIT_ME310_CellularStack.cpp
)
