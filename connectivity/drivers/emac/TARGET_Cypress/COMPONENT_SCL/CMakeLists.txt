# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    PUBLIC
        .
        ./interface
)

target_sources(mbed-emac
    PRIVATE
        interface/default_wifi_interface.cpp
        interface/scl_emac.cpp
        interface/SclAccessPoint.cpp
        interface/SclSTAInterface.cpp
)
