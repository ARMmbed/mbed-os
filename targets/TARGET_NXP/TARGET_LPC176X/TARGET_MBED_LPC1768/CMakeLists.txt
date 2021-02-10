# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mbed-lpc1768 INTERFACE)

target_include_directories(mbed-mbed-lpc1768
    INTERFACE
        .
)

target_link_libraries(mbed-mbed-lpc1768 INTERFACE mbed-lpc176x)

add_library(mbed-lpc1768 INTERFACE)

target_link_libraries(mbed-lpc1768 INTERFACE mbed-mbed-lpc1768)
