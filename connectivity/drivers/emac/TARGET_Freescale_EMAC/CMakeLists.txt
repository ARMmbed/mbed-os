# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-emac-kinetis OBJECT)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os-emac-kinetis
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-kinetis
    PRIVATE
        kinetis_emac.cpp
)

mbed_link_component(mbed-os-emac-kinetis mbed-os-netsocket mbed-os)
