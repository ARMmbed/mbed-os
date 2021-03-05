# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-telit-me310 INTERFACE)

target_include_directories(mbed-cellular-telit-me310
    INTERFACE
        .
)

target_sources(mbed-cellular-telit-me310
    INTERFACE
        TELIT_ME310.cpp
        TELIT_ME310_CellularContext.cpp
        TELIT_ME310_CellularNetwork.cpp
        TELIT_ME310_CellularStack.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-telit-me310)
