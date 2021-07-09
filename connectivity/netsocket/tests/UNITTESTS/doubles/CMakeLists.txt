# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-headers-netsocket INTERFACE)

target_include_directories(mbed-headers-netsocket
    INTERFACE
        ${mbed-os_SOURCE_DIR}/connectivity/netsocket/include
)

add_library(mbed-stubs-netsocket)

target_include_directories(mbed-stubs-netsocket
    PUBLIC
        .
)

target_sources(mbed-stubs-netsocket
    PRIVATE
        CellularInterface_stub.cpp
        MeshInterface_stub.cpp
        NetworkInterfaceDefaults_stub.cpp
        NetworkInterface_stub.cpp
        NetworkStack_stub.cpp
        nsapi_dns_stub.cpp
        SocketAddress_stub.cpp
        SocketStats_Stub.cpp
)

target_link_libraries(mbed-stubs-netsocket
    PRIVATE
        mbed-headers-netsocket
        mbed-headers
        mbed-stubs-headers
        mbed-stubs-rtos
        gtest
)
