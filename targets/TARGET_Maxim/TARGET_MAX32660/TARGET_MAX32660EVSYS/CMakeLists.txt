# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32660evsys INTERFACE)

target_include_directories(mbed-max32660evsys
    INTERFACE
        .
)

target_link_libraries(mbed-max32660evsys
    INTERFACE 
        mbed-max32660
)
