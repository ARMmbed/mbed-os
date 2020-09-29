# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-bc95 OBJECT)

target_include_directories(mbed-os-cellular-quectel-bc95
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-quectel-bc95
    PRIVATE
        QUECTEL_BC95.cpp
        QUECTEL_BC95_CellularContext.cpp
        QUECTEL_BC95_CellularInformation.cpp
        QUECTEL_BC95_CellularNetwork.cpp
        QUECTEL_BC95_CellularStack.cpp
)

target_link_libraries(mbed-os-cellular-quectel-bc95 mbed-os-cellular mbed-os)
