# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-netsocket-api STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-netsocket-api
    PUBLIC
        .
        ./include
        ./include/netsocket
)

target_sources(mbed-netsocket-api
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

target_compile_definitions(mbed-netsocket-api
    PUBLIC
        MBED_CONF_NSAPI_PRESENT=1
)

target_link_libraries(mbed-netsocket-api
    PUBLIC
        mbed-core-flags
        mbed-mbedtls
)

add_library(mbed-netsocket INTERFACE)

target_link_libraries(mbed-netsocket
    INTERFACE
        mbed-mbedtls
        mbed-lwipstack
)

if("DEVICE_EMAC=1" IN_LIST MBED_TARGET_DEFINITIONS)
    target_link_libraries(mbed-netsocket
        INTERFACE
            mbed-emac
    )
endif()

target_link_libraries(mbed-netsocket
    INTERFACE
        mbed-wifi
        mbed-cellular
        mbed-nanostack-libservice
)
