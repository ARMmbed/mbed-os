# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/netsocket
)

target_sources(mbed-os
    PRIVATE
        source/CellularNonIPSocket.cpp
        source/DTLSSocket.cpp
        source/DTLSSocketWrapper.cpp
        source/EMACInterface.cpp
        source/EthernetInterface.cpp
        source/ICMPSocket.cpp
        source/InternetDatagramSocket.cpp
        source/InternetSocket.cpp
        source/L3IPInterface.cpp
        source/NetStackMemoryManager.cpp
        source/NetworkInterface.cpp
        source/NetworkInterfaceDefaults.cpp
        source/NetworkStack.cpp
        source/PPPInterface.cpp
        source/SocketAddress.cpp
        source/SocketStats.cpp
        source/TCPSocket.cpp
        source/TLSSocket.cpp
        source/TLSSocketWrapper.cpp
        source/UDPSocket.cpp
        source/WiFiAccessPoint.cpp
        source/nsapi_dns.cpp
        source/nsapi_ppp.cpp
)
