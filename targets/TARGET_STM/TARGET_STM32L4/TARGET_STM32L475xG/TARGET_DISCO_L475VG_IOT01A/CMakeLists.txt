# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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

set(MBED_BLE_EXTRA_LIBRARIES ${MBED_BLE_EXTRA_LIBRARIES} mbed-ble-bluenrg-ms CACHE INTERNAL "")
