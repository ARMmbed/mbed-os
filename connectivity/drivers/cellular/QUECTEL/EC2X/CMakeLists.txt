# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-quectel-ec2x INTERFACE)

target_include_directories(mbed-cellular-quectel-ec2x
    INTERFACE
        .
)

target_sources(mbed-cellular-quectel-ec2x
    INTERFACE
        QUECTEL_EC2X.cpp
)
