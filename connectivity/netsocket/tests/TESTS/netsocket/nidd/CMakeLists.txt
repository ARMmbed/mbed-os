# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE=CELLULAR" IN_LIST MBED_CONFIG_DEFINITIONS)
	set(TEST_SKIPPED "Target's default network interface type is not CELLULAR.")
endif()


list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            niddsocket_send_timeout.cpp
            niddsocket_send_repeat.cpp
            niddsocket_send_invalid.cpp
            niddsocket_recv_timeout.cpp
            niddsocket_open_twice.cpp
            niddsocket_open_limit.cpp
            niddsocket_open_destruct.cpp
            niddsocket_open_close_repeat.cpp
            niddsocket_echotest.cpp
            niddsocket_disconnect.cpp
            niddsocket_connect.cpp       
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-netsocket-nidd
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_REQUIRED_LIBS
        mbed-netsocket
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
