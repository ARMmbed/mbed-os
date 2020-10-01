# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os-ble
    INTERFACE
        cy_bt_cordio_cfg.cpp
)

target_link_libraries(mbed-os-ble
    INTERFACE
        mbed-os-ble
)
