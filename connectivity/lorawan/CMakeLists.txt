# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-lorawan OBJECT)

add_subdirectory(lorastack)
add_subdirectory(system)

target_include_directories(mbed-lorawan
    PUBLIC
        ..
        .
        ./include
        ./include/lorawan
)

target_sources(mbed-lorawan
    PRIVATE
        source/LoRaWANInterface.cpp
        source/LoRaWANStack.cpp
)

target_compile_definitions(mbed-lorawan
    PUBLIC
        MBED_CONF_LORAWAN_PRESENT=1
)

target_link_libraries(mbed-lorawan
    PRIVATE
        mbed-core
        mbed-events
    PUBLIC
        mbed-mbedtls
)
