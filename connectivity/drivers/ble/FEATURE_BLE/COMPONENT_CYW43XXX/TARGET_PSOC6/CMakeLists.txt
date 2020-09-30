# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43xxx-psoc6 INTERFACE)

target_sources(mbed-os-ble-cyw43xxx-psoc6
    INTERFACE
        cy_bt_cordio_cfg.cpp
)

target_link_libraries(mbed-os-ble-cyw43xxx-psoc6 INTERFACE mbed-os-ble)
