# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-telit-me310 OBJECT)

target_include_directories(mbed-os-cellular-telit-me310
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-telit-me310
    PRIVATE
        TELIT_ME310.cpp
        TELIT_ME310_CellularContext.cpp
        TELIT_ME310_CellularNetwork.cpp
        TELIT_ME310_CellularStack.cpp
)
