# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-utest
    PUBLIC
        .
        utest
		../greentea-client
)

target_sources(mbed-utest
    PRIVATE
        mbed-utest-shim.cpp
        source/unity_handler.cpp
        source/utest_case.cpp
        source/utest_default_handlers.cpp
        source/utest_greentea_handlers.cpp
        source/utest_harness.cpp
        source/utest_print.cpp
        source/utest_shim.cpp
        source/utest_stack_trace.cpp
        source/utest_types.cpp
)

target_link_libraries(mbed-utest PUBLIC mbed-core-flags mbed-unity)
