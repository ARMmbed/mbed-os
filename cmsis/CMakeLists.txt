# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(cmsis OBJECT)

mbed_configure_component(cmsis)

add_subdirectory(CMSIS_5)
add_subdirectory(device)

# Using relative paths behavior
if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()
