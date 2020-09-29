# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-generic-at3gpp OBJECT)

target_include_directories(mbed-os-cellular-generic-at3gpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-generic-at3gpp
    PRIVATE
        GENERIC_AT3GPP.cpp
)

target_link_libraries(mbed-os-cellular-generic-at3gpp mbed-os-cellular mbed-os)
