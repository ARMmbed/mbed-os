# SPDX-License-Identifier: Apache-2.0

add_subdirectory(ppp)

target_sources(mbed-os PRIVATE
    NetworkInterface.cpp
    NetworkInterfaceDefaults.cpp
    NetworkStack.cpp
    nsapi_dns.cpp
    NetStackMemoryManager.cpp
    SocketAddress.cpp
    SocketStats.cpp

    emac-drivers
    EMACInterface.cpp

    EthernetInterface.cpp

    ICMPSocket.cpp
    UDPSocket.cpp
    TCPSocket.cpp
    TLSSocket.cpp
    TLSSocketWrapper.cpp
    DTLSSocket.cpp
    DTLSSocketWrapper.cpp

    InternetDatagramSocket.cpp

    InternetSocket.cpp
)
target_include_directories(mbed-os PUBLIC .)
mbed_add_cmake_directory_if_labels("emac-drivers/TARGET")
