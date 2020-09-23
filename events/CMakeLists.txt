# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events/internal
)

target_sources(mbed-os
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/EventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue_mbed.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue_posix.c
        ${CMAKE_CURRENT_SOURCE_DIR}/source/mbed_shared_queues.cpp
)
