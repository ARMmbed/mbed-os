# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/config
)

target_sources(mbed-os
    PRIVATE
        PeripheralPins.c
        USBPhy_Nordic.cpp
)

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(device)
