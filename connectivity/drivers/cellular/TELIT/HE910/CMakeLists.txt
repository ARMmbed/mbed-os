# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-cellular
    INTERFACE
        .
)

target_sources(mbed-os-cellular
    INTERFACE
        TELIT_HE910.cpp
)
