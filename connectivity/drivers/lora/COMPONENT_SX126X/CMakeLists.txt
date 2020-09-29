# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-lorawan-sx126x OBJECT)

target_include_directories(mbed-os-lorawan-sx126x
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-lorawan-sx126x
    PRIVATE
        SX126X_LoRaRadio.cpp
)

mbed_link_component(mbed-os-lorawan-sx126x mbed-os-lorawan mbed-os)
