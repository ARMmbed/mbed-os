# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-alt1250-ppp INTERFACE)

target_include_directories(mbed-cellular-alt1250-ppp
    INTERFACE
        .
)

target_sources(mbed-cellular-alt1250-ppp
    INTERFACE
        ALT1250_PPP.cpp
        ALT1250_PPP_CellularContext.cpp
        ALT1250_PPP_CellularNetwork.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-alt1250-ppp)
