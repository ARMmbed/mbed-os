# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set(MBED_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}")

include("${MBED_PATH}/tools/cmake/app.cmake")

project(multiple_executables)

add_subdirectory("${MBED_PATH}" "mbed-os-build")

add_subdirectory(app1)
add_subdirectory(app2)
