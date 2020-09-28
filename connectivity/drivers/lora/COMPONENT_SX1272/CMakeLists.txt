# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-lorawan-sx126x OBJECT)

target_include_directories(mbed-os-lorawan-sx1272
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/registers
)

target_sources(mbed-os-lorawan-sx1272
    PRIVATE
        SX1272_LoRaRadio.cpp
)
