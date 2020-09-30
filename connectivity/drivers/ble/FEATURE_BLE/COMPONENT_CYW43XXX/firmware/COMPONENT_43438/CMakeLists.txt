# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-cyw43438 INTERFACE)

target_sources(mbed-os-ble-cyw43438
    INTERFACE
        w_bt_firmware_controller.c
)

target_link_libraries(mbed-os-ble-cyw43438 INTERFACE mbed-os-ble)
