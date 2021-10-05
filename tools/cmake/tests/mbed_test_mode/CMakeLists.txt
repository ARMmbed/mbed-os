# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../.. CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE STATIC "")

include(${MBED_PATH}/tools/cmake/app.cmake)

add_test(NAME mbed-test-mode-check COMMAND mbed-test-mode-check)
add_executable(mbed-test-mode-check)
project(mbed-test-mode-check)

set(PROJECT_NAME ${CMAKE_PROJECT_NAME} CACHE INTERNAL "")
include(CTest)

target_sources(mbed-test-mode-check
    PRIVATE
        main.cpp
)
