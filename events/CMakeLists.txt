# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-events OBJECT)

target_include_directories(mbed-events
    PUBLIC
        .
        ./include
        ./include/events
        ./include/events/internal
)

target_sources(mbed-events
    PRIVATE
        source/EventQueue.cpp
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/mbed_shared_queues.cpp
)

target_compile_definitions(mbed-events
    PUBLIC
        MBED_CONF_EVENTS_PRESENT=1
)

target_link_libraries(mbed-events PRIVATE mbed-core)
