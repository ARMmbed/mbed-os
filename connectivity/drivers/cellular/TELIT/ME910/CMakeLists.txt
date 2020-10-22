# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-cellular
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-cellular
    INTERFACE
        TELIT_ME910.cpp
        TELIT_ME910_CellularContext.cpp
        TELIT_ME910_CellularNetwork.cpp
)
