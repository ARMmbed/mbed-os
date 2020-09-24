# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Extend the cached internal components list
list(APPEND mbed-os-internal-components
    mbed-os-cellular
    mbed-os-ble
    mbed-os-lorawan
    mbed-os-mbedtls
    mbed-os-nanostack
    mbed-os-netsocket
    mbed-os-nfc
)

# Components paths to the main CMake file
set(mbed-os-ble_PATH connectivity/FEATURE_BLE)
set(mbed-os-nfc_PATH connectivity/nfc)
set(mbed-os-cellular_PATH connectivity/cellular)
set(mbed-os-lorawan_PATH connectivity/lorawan)
set(mbed-os-mbedtls_PATH connectivity/mbedtls)
set(mbed-os-nanostack_PATH connectivity/nanostack)
set(mbed-os-netsocket_PATH connectivity/netsocket)
