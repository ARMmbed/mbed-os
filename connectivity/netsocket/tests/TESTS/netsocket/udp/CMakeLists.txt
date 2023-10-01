# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND
        TEST_SOURCE_LIST
            main.cpp
            udpsocket_bind_port.cpp
            udpsocket_bind_address_null.cpp
            udpsocket_bind_address_invalid.cpp
            udpsocket_bind_address.cpp            
            udpsocket_sendto_timeout.cpp
            udpsocket_sendto_repeat.cpp
            udpsocket_sendto_invalid.cpp
            udpsocket_recv_timeout.cpp
            udpsocket_open_twice.cpp
            udpsocket_open_limit.cpp
            udpsocket_open_destruct.cpp
            udpsocket_open_close_repeat.cpp
            udpsocket_echotest.cpp
            udpsocket_bind_wrong_type.cpp
            udpsocket_bind_unopened.cpp
            udpsocket_bind_port_fail.cpp
            udpsocket_echotest_burst.cpp       
)

if(MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "This test requires an RTOS!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-udp
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
