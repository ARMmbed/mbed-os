# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-events)

target_sources(mbed-stubs-events
    PRIVATE
        equeue_stub.c
        ${MBED_PATH}/events/tests/UNITTESTS/stubs/EqueuePosix_stub.c
        EventFlags_stub.cpp
        EventQueue_stub.cpp
        mbed_shared_queues_stub.cpp
)

target_link_libraries(mbed-stubs-events
    PRIVATE
        mbed-headers
        mbed-stubs-headers
)
