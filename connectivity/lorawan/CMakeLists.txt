# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

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

target_compile_definitions(mbed-os-lorawan
    PUBLIC
        MBED_CONF_LORAWAN_PRESENT=1
)

mbed_link_component(mbed-os-lorawan mbed-os mbed-os-events)
