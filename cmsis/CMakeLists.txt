# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(CMSIS_5)
add_subdirectory(device)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MBED_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests/UNITTESTS/doubles)
endif()
