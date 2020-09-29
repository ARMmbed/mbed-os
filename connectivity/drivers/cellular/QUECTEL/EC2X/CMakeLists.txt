# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-ec2x INTERFACE)

target_include_directories(mbed-os-cellular-quectel-ec2x
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-quectel-ec2x
    INTERFACE
        QUECTEL_EC2X.cpp
)

target_link_libraries(mbed-os-cellular-quectel-ec2x INTERFACE mbed-os-cellular)
