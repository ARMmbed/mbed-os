# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/network
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_sources(mbed-os
    PRIVATE
        interface/CyDhcpServer.cpp
        interface/WhdAccessPoint.cpp
        interface/WhdSTAInterface.cpp
        interface/WhdSoftAPInterface.cpp
        interface/whd_emac.cpp
        interface/whd_interface.cpp

        utils/cydhcp_server_debug.cpp
        utils/cynetwork_utils.c
)
