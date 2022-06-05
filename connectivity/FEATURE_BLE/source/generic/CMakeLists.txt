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
        MemorySecurityDb.cpp
        PrivateAddressController.cpp
        SecurityDb.cpp
        SecurityManagerImpl.cpp
)

target_link_libraries(mbed-ble
    PUBLIC
        mbed-storage-kvstore
        mbed-storage-tdbstore
        mbed-storage-kv-global-api
)
