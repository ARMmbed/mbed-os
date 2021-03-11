# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-randlib STATIC)

target_include_directories(mbed-randlib
    PUBLIC
        include
        include/mbed-client-randlib
        include/mbed-client-randlib/platform
)

target_sources(mbed-randlib
    PRIVATE
        source/randLIB.c
)

target_link_libraries(mbed-randlib PRIVATE mbed-mbedtls mbed-nanostack-libservice)
