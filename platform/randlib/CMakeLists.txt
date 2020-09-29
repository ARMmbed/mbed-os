# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-randlib INTERFACE)

target_include_directories(mbed-os-randlib
    INTERFACE
        include
        include/mbed-client-randlib
        include/mbed-client-randlib/platform
)

target_sources(mbed-os-randlib
    INTERFACE
        source/randLIB.c
)

target_link_libraries(mbed-os-randlib INTERFACE mbed-os-nanostack)
