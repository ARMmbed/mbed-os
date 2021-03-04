# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-whd INTERFACE)

target_include_directories(mbed-emac-whd
    INTERFACE
        .
        ./interface
        ./network
        ./utils
)

target_sources(mbed-emac-whd
    INTERFACE
        interface/CyDhcpServer.cpp
        interface/WhdAccessPoint.cpp
        interface/WhdSTAInterface.cpp
        interface/WhdSoftAPInterface.cpp
        interface/whd_emac.cpp
        interface/whd_interface.cpp

        utils/cydhcp_server_debug.cpp
        utils/cynetwork_utils.c
)
