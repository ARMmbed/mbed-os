# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-sara4-ppp OBJECT)

target_include_directories(mbed-os-cellular-sara4-ppp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-sara4-ppp
    PRIVATE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)

mbed_link_component(mbed-os-cellular-sara4-ppp mbed-os-cellular mbed-os)
