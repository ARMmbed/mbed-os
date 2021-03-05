# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-multitech-dragonfly-nano INTERFACE)

target_include_directories(mbed-cellular-multitech-dragonfly-nano
    INTERFACE
        .
)

target_sources(mbed-cellular-multitech-dragonfly-nano
    INTERFACE
        SARA4_PPP.cpp
        SARA4_PPP_CellularNetwork.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-multitech-dragonfly-nano)
