# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-cyw43xxx-43012 INTEFACE)

target_sources(mbed-ble-cyw43xxx-43012
    PRIVATE
        w_bt_firmware_controller.c
)
