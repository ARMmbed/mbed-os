# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw4343w OBJECT)

target_sources(mbed-os-ble-cyw4343w
    PRIVATE
        w_bt_firmware_controller.c
)
