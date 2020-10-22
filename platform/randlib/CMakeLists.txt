# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-randlib INTERFACE)

target_include_directories(mbed-randlib
    INTERFACE
        include
        include/mbed-client-randlib
        include/mbed-client-randlib/platform
)

target_sources(mbed-randlib
    INTERFACE
        source/randLIB.c
)

target_link_libraries(mbed-randlib
    INTERFACE
        mbed-nanostack
)
