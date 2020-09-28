# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-lorawan OBJECT)

add_subdirectory(lorastack)
add_subdirectory(system)

target_include_directories(mbed-os-lorawan
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/lorawan
)

target_sources(mbed-os-lorawan
    PRIVATE
        source/LoRaWANInterface.cpp
        source/LoRaWANStack.cpp
)

target_link_libraries(mbed-os-lorawan mbed-os)
