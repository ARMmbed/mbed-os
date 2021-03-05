# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND MBED_ADD_UNITTEST_SUBDIRS
    CellularNonIPSocket
    DTLSSocket
    DTLSSocketWrapper
    EthernetInterface
    IfaceDnsSocket
    InternetSocket
    NetworkInterface
    NetworkStack
    PPPInterface
    SocketAddress
    TCPSocket
    TLSSocket
    TLSSocketWrapper
    UDPSocket
    WiFiAccessPoint
)

mbed_add_all_subdirectories("${MBED_ADD_UNITTEST_SUBDIRS}")
