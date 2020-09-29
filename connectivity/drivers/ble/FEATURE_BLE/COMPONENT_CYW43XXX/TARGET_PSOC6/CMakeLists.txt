# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43xxx INTERFACE)

target_sources(mbed-os-ble-cyw43xxx
    INTERFACE
        cy_bt_cordio_cfg.cpp
)
