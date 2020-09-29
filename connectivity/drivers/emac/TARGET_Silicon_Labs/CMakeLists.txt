# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-sl INTERFACE)

target_include_directories(mbed-os-emac-sl
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-sl
    INTERFACE
        sl_emac.cpp
        sl_eth_hw.c
)

target_link_libraries(mbed-os-emac-sl INTERFACE mbed-os-netsocket)
