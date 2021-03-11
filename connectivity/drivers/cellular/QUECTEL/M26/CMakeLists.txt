# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular-quectel-m26 INTERFACE)

target_include_directories(mbed-cellular-quectel-m26
    INTERFACE
        .
)

target_sources(mbed-cellular-quectel-m26
    INTERFACE
        QUECTEL_M26.cpp
        QUECTEL_M26_CellularContext.cpp
        QUECTEL_M26_CellularInformation.cpp
        QUECTEL_M26_CellularStack.cpp
)

target_link_libraries(mbed-cellular PRIVATE mbed-cellular-quectel-m26)
