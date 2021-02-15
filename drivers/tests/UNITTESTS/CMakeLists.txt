# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/mbed_unittest.cmake)

list(APPEND MBED_ADD_UNITTEST_SUBDIRS
    PwmOut
    Watchdog
)

mbed_add_all_subdirectories("${MBED_ADD_UNITTEST_SUBDIRS}")
