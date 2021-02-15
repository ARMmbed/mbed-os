# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.19.0)

set(PROJECT_NAME mbed-drivers-unittests)

project(${PROJECT_NAME})

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../ CACHE INTERNAL "")

include(${MBED_PATH}/tools/cmake/mbed_unittest.cmake)

mbed_unittest_setup()

## add mbed-os cmakelists
add_subdirectory(${MBED_PATH}/UNITTESTS/stubs ${CMAKE_CURRENT_BINARY_DIR}/stubs_build)
add_subdirectory(${MBED_PATH}/UNITTESTS/stubs/platform ${CMAKE_CURRENT_BINARY_DIR}/stubs_platform_build)
add_subdirectory(${MBED_PATH}/UNITTESTS/stubs/hal ${CMAKE_CURRENT_BINARY_DIR}/stubs_hal_build)
add_subdirectory(${MBED_PATH}/UNITTESTS/fakes/drivers ${CMAKE_CURRENT_BINARY_DIR}/fakes_build)

## all directories here will be added
add_subdirectory(UNITTESTS)
