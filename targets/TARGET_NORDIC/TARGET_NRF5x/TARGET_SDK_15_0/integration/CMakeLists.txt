# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/nrfx/legacy/ble_flash
)

target_sources(mbed-core
    INTERFACE
        nrfx/legacy/nrf_drv_rng.c
        nrfx/legacy/nrf_drv_usbd.c

        nrfx/legacy/ble_flash/ble_flash.c
)
