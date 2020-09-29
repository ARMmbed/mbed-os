# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-telit-me910 INTERFACE)

target_include_directories(mbed-os-cellular-telit-me910
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-telit-me910
    INTERFACE
        TELIT_ME910.cpp
        TELIT_ME910_CellularContext.cpp
        TELIT_ME910_CellularNetwork.cpp
)

target_link_libraries(mbed-os-cellular-telit-me910 INTERFACE mbed-os-cellular)
