# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    INTERFACE
        .
)

target_sources(mbed-cellular
    INTERFACE
        QUECTEL_BG96.cpp
        QUECTEL_BG96_CellularContext.cpp
        QUECTEL_BG96_CellularInformation.cpp
        QUECTEL_BG96_CellularNetwork.cpp
        QUECTEL_BG96_CellularStack.cpp
        QUECTEL_BG96_ControlPlane_netif.cpp
)
