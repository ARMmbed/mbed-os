# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-telit-he910 INTERFACE)

target_include_directories(mbed-cellular-telit-he910
    INTERFACE
        .
)

target_sources(mbed-cellular-telit-he910
    INTERFACE
        TELIT_HE910.cpp
)
