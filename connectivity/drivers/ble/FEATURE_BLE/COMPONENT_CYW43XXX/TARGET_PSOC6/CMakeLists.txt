# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43xxx OBJECT)

target_sources(mbed-os-ble-cyw43xxx
    PRIVATE
        cy_bt_cordio_cfg.cpp
)
