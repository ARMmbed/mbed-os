# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    else()
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

if("EXPERIMENTAL_API" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(FEATURE_EXPERIMENTAL_API)
endif()

add_subdirectory(cxxsupport)
add_subdirectory(mbed-trace)
add_subdirectory(randlib)
add_subdirectory(source)

target_include_directories(mbed-core-flags
    INTERFACE
        include
        include/platform
        include/platform/internal
)

