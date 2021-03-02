# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cyw43xxx-psoc6 INTERFACE)

target_sources(mbed-ble-cyw43xxx-psoc6
    PRIVATE
        cy_bt_cordio_cfg.cpp
)
