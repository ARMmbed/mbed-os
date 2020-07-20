# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/EventQueue.cpp
        source/mbed_shared_queues.cpp
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        internal
)
