# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(lorastack)
add_subdirectory(system)

target_include_directories(mbed-os-lorawan
    INTERFACE
        .
        ./include
        ./include/lorawan
)

target_sources(mbed-os-lorawan
    INTERFACE
        source/LoRaWANInterface.cpp
        source/LoRaWANStack.cpp
)

target_compile_definitions(mbed-os-lorawan
    INTERFACE
        MBED_CONF_LORAWAN_PRESENT=1
)

target_link_libraries(mbed-os-lorawan
    INTERFACE
        mbed-os-events
)
