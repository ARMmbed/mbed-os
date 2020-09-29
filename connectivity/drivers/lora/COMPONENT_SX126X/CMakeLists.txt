# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-lorawan-sx126x INTERFACE)

target_include_directories(mbed-os-lorawan-sx126x
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-lorawan-sx126x
    INTERFACE
        SX126X_LoRaRadio.cpp
)

target_link_libraries(mbed-os-lorawan-sx126x INTERFACE mbed-os-lorawan)
