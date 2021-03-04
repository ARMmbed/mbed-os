# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-scl INTERFACE)

target_include_directories(mbed-emac-scl
    INTERFACE
        .
        ./interface
)

target_sources(mbed-emac-scl
    INTERFACE
        interface/default_wifi_interface.cpp
        interface/scl_emac.cpp
        interface/SclAccessPoint.cpp
        interface/SclSTAInterface.cpp
)
