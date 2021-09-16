# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if(NOT BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_subdirectory(mbed-coap)
add_subdirectory(nanostack-libservice)
add_subdirectory(ppp)
