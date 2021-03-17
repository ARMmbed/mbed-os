# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME tdbstore-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME} 
    PRIVATE
        UNITTEST
)

target_sources(${TEST_NAME}
    PRIVATE
        ${MBED_PATH}/storage/blockdevice/source/HeapBlockDevice.cpp
        ${MBED_PATH}/storage/blockdevice/source/BufferedBlockDevice.cpp
        ${MBED_PATH}/storage/kvstore/tdbstore/source/TDBStore.cpp      
        moduletest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers
        mbed-stubs-platform        
        gmock_main
)

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "storage")
