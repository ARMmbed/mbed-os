# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND connectivity-components
    mbed-os-cellular
    mbed-os-ble
    mbed-os-lorawan
    mbed-os-mbedtls
    mbed-os-nanostack
    mbed-os-netsocket
    mbed-os-nfc
)

# only BLE folder is named differently
set(mbed-os-ble_PATH connectivity/FEATURE_BLE)
set(mbed-os-nfc_PATH connectivity/nfc)
