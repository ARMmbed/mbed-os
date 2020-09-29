# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-netsocket OBJECT)

target_include_directories(mbed-os-netsocket
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/netsocket
)

get_property(_internal_components_enabled GLOBAL PROPERTY mbed-os-internal-components-enabled)
if(mbed-os-cellular IN_LIST _internal_components_enabled)
target_sources(mbed-os-netsocket
    PRIVATE
        source/CellularNonIPSocket.cpp
)

endif()

target_sources(mbed-os-netsocket
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

target_compile_definitions(mbed-os-netsocket
    PUBLIC
        MBED_CONF_NETSOCKET_PRESENT=1
)

target_link_libraries(mbed-os-netsocket mbed-os-nanostack-libservice mbed-os-mbedtls mbed-os)
