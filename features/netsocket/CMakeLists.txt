# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    #CellularInterface.h
    #CellularNonIPSocket.cpp
    #CellularNonIPSocket.h
    #OnboardNetworkStack.h
    #ppp
    #PPP.h
    #PPPInterface.cpp
    #PPPInterface.h

    NetworkInterface.cpp
    NetworkInterfaceDefaults.cpp
    NetworkInterface.h
    NetworkStack.cpp
    NetworkStack.h
    nsapi_dns.cpp
    nsapi_dns.h
    nsapi.h
    nsapi_ppp.h
    nsapi_types.h
    NetStackMemoryManager.cpp
    NetStackMemoryManager.h
    ControlPlane_netif.h
    SocketAddress.cpp
    SocketAddress.h
    Socket.h
    SocketStats.cpp
    SocketStats.h

    emac-drivers
    EMAC.h
    EMACInterface.cpp
    EMACInterface.h
    EMACMemoryManager.h

    EthernetInterface.cpp
    EthernetInterface.h
    EthInterface.h

    DNS.h
    ICMPSocket.cpp
    ICMPSocket.h
    UDPSocket.cpp
    UDPSocket.h
    TCPServer.cpp
    TCPServer.h
    TCPSocket.cpp
    TCPSocket.h
    TLSSocket.cpp
    TLSSocket.h
    TLSSocketWrapper.cpp
    TLSSocketWrapper.h
    DTLSSocket.cpp
    DTLSSocket.h
    DTLSSocketWrapper.cpp
    DTLSSocketWrapper.h

    InternetDatagramSocket.cpp
    InternetDatagramSocket.h

    InternetSocket.cpp
    InternetSocket.h

    #L3IP.h
    #L3IPInterface.cpp
    #L3IPInterface.h

    #MeshInterface.h


    #WiFiAccessPoint.cpp
    #WiFiAccessPoint.h
    #WiFiInterface.h
)
target_include_directories(mbed-os PUBLIC .)
mbed_add_cmake_directory_if_labels("emac-drivers/TARGET")
