# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-bg96 OBJECT)

target_include_directories(mbed-os-cellular-quectel-bg96
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-quectel-bg96
    PRIVATE
        QUECTEL_BG96.cpp
        QUECTEL_BG96_CellularContext.cpp
        QUECTEL_BG96_CellularInformation.cpp
        QUECTEL_BG96_CellularNetwork.cpp
        QUECTEL_BG96_CellularStack.cpp
        QUECTEL_BG96_ControlPlane_netif.cpp
)

target_link_libraries(mbed-os-cellular-quectel-bg96 mbed-os-cellular mbed-os)
