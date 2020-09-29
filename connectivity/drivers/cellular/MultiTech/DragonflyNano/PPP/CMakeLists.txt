# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-sara4-ppp INTERFACE)

target_include_directories(mbed-os-cellular-sara4-ppp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-sara4-ppp
    INTERFACE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)

target_link_libraries(mbed-os-cellular-sara4-ppp INTERFACE mbed-os-cellular)
