# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-lpc546xx INTERFACE)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-lpc546xx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-lpc546xx
    INTERFACE
        lpc546xx_emac.cpp
)

target_link_libraries(mbed-os-emac-lpc546xx INTERFACE mbed-os-netsocket)
