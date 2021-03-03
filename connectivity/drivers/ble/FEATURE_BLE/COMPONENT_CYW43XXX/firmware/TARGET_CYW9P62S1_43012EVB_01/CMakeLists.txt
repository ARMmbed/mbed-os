# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cyw9p62s1_43012evb_01 INTERFACE)

target_sources(mbed-ble-cyw9p62s1_43012evb_01
    INTERFACE
        w_bt_firmware_controller.c
)
