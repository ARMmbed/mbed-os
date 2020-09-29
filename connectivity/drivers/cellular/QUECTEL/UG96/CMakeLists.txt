# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-ug96 INTERFACE)

target_sources(mbed-os-cellular-quectel-ug96
    INTERFACE
        QUECTEL_UG96.cpp
        QUECTEL_UG96_CellularContext.cpp
)

target_include_directories(mbed-os-cellular-quectel-ug96
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mbed-os-cellular-quectel-ug96 INTERFACE mbed-os-cellular)
