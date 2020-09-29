# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-emac-smsc9220 OBJECT)

target_include_directories(mbed-os-emac-smsc9220
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-smsc9220 
    PRIVATE
        smsc9220_emac.cpp
)

mbed_link_component(mbed-os-emac-smsc9220 mbed-os-netsocket mbed-os)
