# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-lorawan-sx1272 OBJECT)

target_include_directories(mbed-os-lorawan-sx1272
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/registers
)

target_sources(mbed-os-lorawan-sx1272
    PRIVATE
        SX1272_LoRaRadio.cpp
)

mbed_link_component(mbed-os-lorawan-sx1272 mbed-os-lorawan mbed-os)
