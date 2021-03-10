# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lorawan-sx126x INTERFACE)

target_include_directories(mbed-lorawan-sx126x
    INTERFACE
        .
)

target_sources(mbed-lorawan-sx126x
    INTERFACE
        SX126X_LoRaRadio.cpp
)

target_link_libraries(mbed-lorawan
    PRIVATE
        mbed-lorawan-sx126x
)

target_include_directories(mbed-lorawan
    PUBLIC
        $<TARGET_PROPERTY:mbed-lorawan-sx126x,INTERFACE_INCLUDE_DIRECTORIES>
)
