# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-ec2x OBJECT)

target_include_directories(mbed-os-cellular-quectel-ec2x
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-quectel-ec2x
    PRIVATE
        QUECTEL_EC2X.cpp
)
