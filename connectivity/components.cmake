# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND connectivity-components
    cellular
    ble
    lorawan
    mbedtls
    nanostack
    netsocket
    nfc
)

# only BLE folder is named differently
set(ble_PATH FEATURE_BLE)
