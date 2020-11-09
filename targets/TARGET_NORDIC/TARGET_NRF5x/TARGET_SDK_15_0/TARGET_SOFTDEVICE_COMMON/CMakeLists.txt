# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


target_include_directories(mbed-core
    INTERFACE
        ./ble
        ./ble/ble_radio_notification
        ./libraries/bootloader
        ./libraries/bootloader/dfu
        ./libraries/fstorage
)

target_sources(mbed-core
    INTERFACE
        ble/ble_radio_notification/ble_radio_notification.c

        libraries/bootloader/dfu/nrf_dfu_mbr.c

        libraries/fstorage/nrf_fstorage_sd.c
)
