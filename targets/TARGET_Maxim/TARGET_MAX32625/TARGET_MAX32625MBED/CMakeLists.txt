# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32625mbed INTERFACE)

target_include_directories(mbed-max32625mbed
    INTERFACE
        .
)

target_link_libraries(mbed-max32625mbed INTERFACE mbed-max32625 mbed-max32625-no-boot)
