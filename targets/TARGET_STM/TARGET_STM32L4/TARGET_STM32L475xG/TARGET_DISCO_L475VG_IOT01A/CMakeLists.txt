# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/components.cmake)

add_library(mbed-disco-l475vg-iot01a INTERFACE)

target_sources(mbed-disco-l475vg-iot01a
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-l475vg-iot01a
    INTERFACE
        .
)

target_link_libraries(mbed-disco-l475vg-iot01a INTERFACE mbed-stm32l475xg)

mbed_add_component_libraries(mbed-ble mbed-ble-bluenrg_ms)
