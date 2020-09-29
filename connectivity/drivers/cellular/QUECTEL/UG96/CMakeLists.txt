# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-quectel-ug96 OBJECT)

target_sources(mbed-os-cellular-quectel-ug96
    PRIVATE
        QUECTEL_UG96.cpp
        QUECTEL_UG96_CellularContext.cpp
)

target_include_directories(mbed-os-cellular-quectel-ug96
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mbed-os-cellular-quectel-ug96 mbed-os-cellular mbed-os)
