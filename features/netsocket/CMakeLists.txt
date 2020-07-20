# SPDX-License-Identifier: Apache-2.0

add_subdirectory(emac-drivers)
add_subdirectory(ppp)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        CellularNonIPSocket.cpp
        DTLSSocket.cpp
        DTLSSocketWrapper.cpp
        EMACInterface.cpp
        EthernetInterface.cpp
        ICMPSocket.cpp
        InternetDatagramSocket.cpp
        InternetSocket.cpp
        L3IPInterface.cpp
        NetStackMemoryManager.cpp
        NetworkInterface.cpp
        NetworkInterfaceDefaults.cpp
        NetworkStack.cpp
        PPPInterface.cpp
        SocketAddress.cpp
        SocketStats.cpp
        TCPSocket.cpp
        TLSSocket.cpp
        TLSSocketWrapper.cpp
        UDPSocket.cpp
        WiFiAccessPoint.cpp
        nsapi_dns.cpp
)
