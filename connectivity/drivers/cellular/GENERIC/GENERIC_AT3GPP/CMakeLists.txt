# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-generic-at3gpp INTERFACE)

target_include_directories(mbed-os-cellular-generic-at3gpp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-generic-at3gpp
    INTERFACE
        GENERIC_AT3GPP.cpp
)

target_link_libraries(mbed-os-cellular-generic-at3gpp INTERFACE mbed-os-cellular)
