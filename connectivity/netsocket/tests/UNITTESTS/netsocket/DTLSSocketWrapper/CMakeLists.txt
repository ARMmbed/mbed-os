# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME netsocket-dtls-socket-wrapper-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE        
        MBED_CONF_NSAPI_DNS_ADDRESSES_LIMIT=10
)

target_sources(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}/connectivity/netsocket/source/SocketAddress.cpp
        ${MBED_PATH}/connectivity/netsocket/source/NetworkStack.cpp
        ${MBED_PATH}/connectivity/netsocket/source/InternetSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/InternetDatagramSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/UDPSocket.cpp
        ${MBED_PATH}/connectivity/netsocket/source/DTLSSocketWrapper.cpp
        ${MBED_PATH}/connectivity/netsocket/source/TLSSocketWrapper.cpp
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/ip4tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/ip6tos.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip4string/stoip4.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libip6string/stoip6.c
        ${MBED_PATH}/connectivity/libraries/nanostack-libservice/source/libBits/common_functions.c
        test_DTLSSocketWrapper.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers
        mbed-stubs
        mbed-stubs-headers
        gmock_main
)


set(MBEDTLS_USER_CONFIG_FILE_PATH "\"${MBED_PATH}/connectivity/netsocket/tests/UNITTESTS/netsocket/DTLSSocketWrapper/dtls_test_config.h\"")
set_source_files_properties(${MBED_PATH}/connectivity/netsocket/tests/UNITTESTS/netsocket/DTLSSocketWrapper/test_DTLSSocketWrapper.cpp PROPERTIES COMPILE_DEFINITIONS MBEDTLS_USER_CONFIG_FILE=${MBEDTLS_USER_CONFIG_FILE_PATH})
set_source_files_properties(${MBED_PATH}/connectivity/netsocket/source/DTLSSocketWrapper.cpp PROPERTIES COMPILE_DEFINITIONS MBEDTLS_USER_CONFIG_FILE=${MBEDTLS_USER_CONFIG_FILE_PATH})
set_source_files_properties(${MBED_PATH}/connectivity/netsocket/source/TLSSocketWrapper.cpp PROPERTIES COMPILE_DEFINITIONS MBEDTLS_USER_CONFIG_FILE=${MBEDTLS_USER_CONFIG_FILE_PATH})

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "netsocket")
