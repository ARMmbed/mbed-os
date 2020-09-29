# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-imx INTERFACE)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-imx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-imx
    INTERFACE
        imx_emac.cpp
)
