# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ble
        ${CMAKE_CURRENT_SOURCE_DIR}/ble/ble_radio_notification
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/bootloader
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/bootloader/dfu
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/fstorage
)

target_sources(mbed-os
    PRIVATE
        ble/ble_radio_notification/ble_radio_notification.c

        libraries/bootloader/dfu/nrf_dfu_mbr.c

        libraries/fstorage/nrf_fstorage_sd.c
)
