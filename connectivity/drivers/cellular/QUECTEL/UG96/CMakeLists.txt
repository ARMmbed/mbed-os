# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-quectel-ug96 INTERFACE)

target_include_directories(mbed-cellular-quectel-ug96
    INTERFACE
        .
)

target_sources(mbed-cellular-quectel-ug96
    INTERFACE
        QUECTEL_UG96.cpp
        QUECTEL_UG96_CellularContext.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-quectel-ug96)
