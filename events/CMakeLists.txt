# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/util.cmake)

add_library(mbed-events INTERFACE)
add_library(mbed-events-obj OBJECT)

target_include_directories(mbed-events
    INTERFACE
        .
        ./include
        ./include/events
        ./include/events/internal
)

target_sources(mbed-events-obj
    PRIVATE
        source/EventQueue.cpp
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/mbed_shared_queues.cpp
)

target_compile_definitions(mbed-events
    INTERFACE
        MBED_CONF_EVENTS_PRESENT=1
)

mbed_attach_object_lib_with_interface_lib(mbed-events)

target_link_libraries(mbed-events-obj PRIVATE mbed-core)
