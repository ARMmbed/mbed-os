# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-emac-sl OBJECT)

target_include_directories(mbed-os-emac-sl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-sl
    PRIVATE
        sl_emac.cpp
        sl_eth_hw.c
)

mbed_link_component(mbed-os-emac-sl mbed-os-netsocket mbed-os)
