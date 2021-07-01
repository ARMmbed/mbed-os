# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-emac
    INTERFACE
        .
        ./interface
        ./network
        ./utils
)

target_sources(mbed-emac
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
