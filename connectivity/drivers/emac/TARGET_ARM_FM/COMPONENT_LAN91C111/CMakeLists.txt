# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-lan91c111 OBJECT)

target_include_directories(mbed-os-emac-lan91c111
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-lan91c111
    INTERFACE
        fvp_emac.cpp
)

target_link_libraries(mbed-os-emac-lan91c111 INTERFACE mbed-os-netsocket)
