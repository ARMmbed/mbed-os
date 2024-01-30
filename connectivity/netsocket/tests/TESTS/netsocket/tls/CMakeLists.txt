# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            tlssocket_simultaneous.cpp
            tlssocket_send_unconnected.cpp
            tlssocket_send_timeout.cpp
            tlssocket_send_repeat.cpp
            tlssocket_send_closed.cpp
            tlssocket_recv_timeout.cpp
            tlssocket_open_twice.cpp
            tlssocket_open_limit.cpp
            tlssocket_open_destruct.cpp
            tlssocket_no_cert.cpp
            tlssocket_handshake_invalid.cpp
            tlssocket_endpoint_close.cpp
            tlssocket_echotest.cpp
            tlssocket_echotest_burst.cpp
            tlssocket_connect_invalid.cpp
            tlssocket_cert_in_filesystem.cpp
)

if(MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "This test requires an RTOS!")
endif()

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "This test requires TRNG support!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-tls
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
        mbed-storage-blockdevice
    mbed-storage-littlefs
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
