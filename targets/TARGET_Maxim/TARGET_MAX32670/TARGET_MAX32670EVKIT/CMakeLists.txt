# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32670evkit INTERFACE)

target_include_directories(mbed-max32670evkit
    INTERFACE
        .
)

target_link_libraries(mbed-max32670evkit
    INTERFACE 
        mbed-max32670
)
