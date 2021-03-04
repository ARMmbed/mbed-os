# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cyw9p62s1-43012evb-01 INTERFACE)

target_sources(mbed-ble-cyw9p62s1-43012evb-01
    INTERFACE
        w_bt_firmware_controller.c
)

target_link_libraries(mbed-ble-cyw9p62s1-43012evb-01 INTERFACE mbed-ble-cyw43xxx)
