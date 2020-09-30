# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-ble-cyw43xxx-cyw9p62s1_43012evb_01
    INTERFACE
        w_bt_firmware_controller.c
)

target_link_libraries(mbed-os-ble-cyw43xxx-cyw9p62s1_43012evb_01 INTERFACE mbed-os-ble)
