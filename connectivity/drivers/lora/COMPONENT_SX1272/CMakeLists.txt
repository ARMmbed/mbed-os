# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lorawan-sx1272 INTERFACE)

target_include_directories(mbed-lorawan-sx1272
    INTERFACE
        .
        ./registers
)

target_sources(mbed-lorawan-sx1272
    INTERFACE
        SX1272_LoRaRadio.cpp
)

target_link_libraries(mbed-lorawan
    PRIVATE
        mbed-lorawan-sx1272
)

target_include_directories(mbed-lorawan
    PUBLIC
        $<TARGET_PROPERTY:mbed-lorawan-sx1272,INTERFACE_INCLUDE_DIRECTORIES>
)
