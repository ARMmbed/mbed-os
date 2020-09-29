# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-rm1000 INTERFACE)

target_include_directories(mbed-os-cellular-rm1000
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-rm1000
    INTERFACE
        RM1000_AT.cpp
        RM1000_AT_CellularContext.cpp
        RM1000_AT_CellularNetwork.cpp
        RM1000_AT_CellularStack.cpp
)

target_link_libraries(mbed-os-cellular-rm1000 INTERFACE mbed-os-cellular)
