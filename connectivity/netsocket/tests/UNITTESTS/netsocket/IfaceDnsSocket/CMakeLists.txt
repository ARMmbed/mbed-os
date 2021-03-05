# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME netsocket-iface-dns-socket-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE        
        MBED_CONF_NSAPI_DNS_ADDRESSES_LIMIT=10
        DEVICE_EMAC
        MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=ETHERNET
        MBED_CONF_NSAPI_DNS_RESPONSE_WAIT_TIME=10000
        MBED_CONF_NSAPI_DNS_RETRIES=1
        MBED_CONF_NSAPI_DNS_TOTAL_ATTEMPTS=10
        MBED_CONF_NSAPI_DNS_CACHE_SIZE=5
)

target_sources(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}/connectivity/netsocket/source/SocketAddress.cpp
        ${MBED_PATH}/connectivity/netsocket/source/NetworkInterface.cpp
        ${MBED_PATH}/connectivity/netsocket/source/NetworkInterfaceDefaults.cpp
        ${MBED_PATH}/connectivity/netsocket/source/NetworkStack.cpp #nsapi_create_stack
        ${MBED_PATH}/connectivity/netsocket/source/InternetSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/TCPSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/InternetDatagramSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/UDPSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/SocketStats.cpp
        ${MBED_PATH}/connectivity/netsocket/source/EthernetInterface.cpp
        ${MBED_PATH}/connectivity/netsocket/source/EMACInterface.cpp
        ${MBED_PATH}/connectivity/netsocket/source/nsapi_dns.cpp
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libList/ns_list.c
        moduletest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers
        mbed-stubs
        mbed-stubs-headers
        gmock_main
)

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "netsocket")
