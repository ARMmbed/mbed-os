# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(NOT MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_subdirectory(mbed-coap)
add_subdirectory(nanostack-libservice)
add_subdirectory(ppp)
