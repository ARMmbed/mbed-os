# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        BLE.cpp
        BLEInstanceBase.cpp
        DiscoveredCharacteristic.cpp
        GattClient.tpp
        GattServer.tpp
        SecurityManager.tpp

        gap/AdvertisingDataBuilder.cpp
        gap/AdvertisingParameters.cpp
        gap/ConnectionParameters.cpp
        gap/Gap.tpp

        generic/FileSecurityDb.cpp
        generic/GenericGap.tpp
        generic/GenericGattClient.tpp
        generic/GenericSecurityManager.tpp
)
