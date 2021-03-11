# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-generic-at3gpp INTERFACE)

target_include_directories(mbed-cellular-generic-at3gpp
    INTERFACE
        .
)

target_sources(mbed-cellular-generic-at3gpp
    INTERFACE
        GENERIC_AT3GPP.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-generic-at3gpp)
