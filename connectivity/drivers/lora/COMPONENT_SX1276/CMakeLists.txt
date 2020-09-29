# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-lorawan-sx1276 INTERFACE)

target_include_directories(mbed-os-lorawan-sx1276
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/registers
)

target_sources(mbed-os-lorawan-sx1276
    INTERFACE
        SX1276_LoRaRadio.cpp
)

target_link_libraries(mbed-os-lorawan-sx1276 INTERFACE mbed-os-lorawan)
