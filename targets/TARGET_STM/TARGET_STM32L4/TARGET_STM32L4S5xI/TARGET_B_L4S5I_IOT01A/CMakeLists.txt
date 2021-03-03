# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/components.cmake)

add_library(mbed-b-l4s5i-iot01a INTERFACE)

target_sources(mbed-b-l4s5i-iot01a
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-b-l4s5i-iot01a
    INTERFACE
        .
)

target_link_libraries(mbed-b-l4s5i-iot01a INTERFACE mbed-stm32l4s5xi)

mbed_add_component_libraries(mbed-ble mbed-ble-bluenrg_ms)
