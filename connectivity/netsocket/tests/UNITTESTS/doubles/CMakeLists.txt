# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-netsocket)

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
        mbed-stubs-rtos-headers
        mbed-headers
        mbed-stubs-headers
        mbed-stubs-rtos
        gtest
)
