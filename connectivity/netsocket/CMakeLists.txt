# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-netsocket OBJECT)

# TODO CMake: Perhaps move this/these file(s) into connectivity/drivers/cellular
target_sources(mbed-cellular
    PRIVATE
        source/CellularNonIPSocket.cpp
)

target_include_directories(mbed-netsocket
    PUBLIC
        .
        ./include
        ./include/netsocket
)

target_sources(mbed-netsocket
    PRIVATE
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

target_compile_definitions(mbed-netsocket
    PUBLIC
        MBED_CONF_NSAPI_PRESENT=1
)

target_link_libraries(mbed-netsocket
    PRIVATE
        mbed-nanostack
        mbed-randlib
        mbed-events
        mbed-core
        mbed-rtos
        mbed-mbedtls
        mbed-lwipstack
        mbed-nanostack-libservice
        mbed-ppp
)
