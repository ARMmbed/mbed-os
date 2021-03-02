# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-ble
    PUBLIC
        .
)

target_sources(mbed-ble
    PRIVATE
        FileSecurityDb.cpp
        GapImpl.cpp
        GattClientImpl.cpp
        KVStoreSecurityDb.cpp
        PrivateAddressController.cpp
        SecurityManagerImpl.cpp
)
