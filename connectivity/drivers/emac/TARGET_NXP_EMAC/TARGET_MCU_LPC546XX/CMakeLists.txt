# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-emac-lpc546xx OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-lpc546xx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-lpc546xx
    PRIVATE
        lpc546xx_emac.cpp
)

mbed_link_component(mbed-os-emac-lpc546xx mbed-os-netsocket mbed-os)
