# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

add_library(events OBJECT)

mbed_configure_component(events)

target_compile_definitions(events
    PUBLIC
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

target_include_directories(events
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events/internal
)

target_sources(events
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/EventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue.c
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue_mbed.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/equeue_posix.c
        ${CMAKE_CURRENT_SOURCE_DIR}/source/mbed_shared_queues.cpp
)

# events DEPENDS cmsis
get_property(cmsis_includes TARGET cmsis PROPERTY INCLUDE_DIRECTORIES)
target_include_directories(events
    PRIVATE
        ${cmsis_includes}
)
