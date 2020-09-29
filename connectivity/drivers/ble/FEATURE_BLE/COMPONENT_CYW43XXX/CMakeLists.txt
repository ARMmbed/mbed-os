# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43xxx OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(firmware)

target_include_directories(mbed-os-ble-cyw43xxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-ble-cyw43xxx
    PRIVATE
        CyH4TransportDriver.cpp
        HCIDriver.cpp
)

target_link_libraries(mbed-os-ble-cyw43xxx mbed-os-ble mbed-os)
