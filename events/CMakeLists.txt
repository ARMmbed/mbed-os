# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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

set_property(TARGET mbed-events PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-events-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-events-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-events,INTERFACE_${options}>)
endforeach()

target_link_libraries(mbed-events-obj PRIVATE mbed-core)
