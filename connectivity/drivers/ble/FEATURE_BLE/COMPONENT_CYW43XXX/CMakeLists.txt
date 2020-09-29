# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43xxx INTERFACE)

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(firmware)

target_include_directories(mbed-os-ble-cyw43xxx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-ble-cyw43xxx
    INTERFACE
        CyH4TransportDriver.cpp
        HCIDriver.cpp
)

target_link_libraries(mbed-os-ble-cyw43xxx INTERFACE mbed-os-ble)
