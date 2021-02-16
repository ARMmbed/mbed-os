# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    INTERFACE
        .
        ./interface
)

target_sources(mbed-emac
    INTERFACE
        interface/default_wifi_interface.cpp
        interface/scl_emac.cpp
        interface/SclAccessPoint.cpp
        interface/SclSTAInterface.cpp
)
