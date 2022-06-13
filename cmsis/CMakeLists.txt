# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(CMSIS_5)
add_subdirectory(device)

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    add_subdirectory(tests/UNITTESTS/doubles)
endif()
