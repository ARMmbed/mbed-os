# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    # target_link_libraries(mbed-os gcov nanostack-libservice)
elseif(DEFINED TARGET_LIKE_X86_OSX_NATIVE)
    add_definitions("-g -O0")
    # target_link_libraries(mbed-os nanostack-libservice)
else()
    # target_link_libraries(mbed-os nanostack-libservice)
endif()

target_include_directories(mbed-os
    PUBLIC
        include
        include/mbed-trace
)

target_sources(mbed-os
    PRIVATE
        source/mbed_trace.c
)
