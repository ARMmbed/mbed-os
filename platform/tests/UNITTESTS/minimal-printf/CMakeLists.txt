# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(TEST_NAME minimal-printf-unittest)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/platform/source/minimal-printf/mbed_printf_implementation.c
        test_minimal-printf-implementation.cpp
)

target_include_directories(mbed-headers-platform
    INTERFACE
        ${mbed-os_SOURCE_DIR}/platform/source/minimal-printf
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-stubs-platform
        gmock_main
)

add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})

set_tests_properties(${TEST_NAME} PROPERTIES LABELS "platform")
