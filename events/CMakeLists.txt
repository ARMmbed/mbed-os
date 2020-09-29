# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-events INTERFACE)

target_include_directories(mbed-os-events
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events/internal
)

target_sources(mbed-os-events
    INTERFACE
        source/EventQueue.cpp
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/mbed_shared_queues.cpp
)

target_compile_definitions(mbed-os-events
    INTERFACE
        MBED_CONF_EVENTS_PRESENT=1
)
