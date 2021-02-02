# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-ble-fake)
add_library(mbed-os-event-queue-fake)

target_include_directories(mbed-os-ble-fake
    PUBLIC
        ${MBED_PATH}/UNITTESTS/fakes/ble
)

target_sources(mbed-os-ble-fake
    PRIVATE
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/gap/AdvertisingDataBuilder.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/gap/AdvertisingParameters.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/gap/ConnectionParameters.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/gatt/DiscoveredCharacteristic.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/Gap.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/GattClient.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/GattServer.cpp
        ${MBED_PATH}/connectivity/FEATURE_BLE/source/SecurityManager.cpp
        ${MBED_PATH}/UNITTESTS/fakes/ble/BLE.cpp
)

target_link_libraries(mbed-os-ble-fake
    PRIVATE
        mbed-os-stubs-headers
)

target_sources(mbed-os-event-queue-fake
    PRIVATE
        ${MBED_PATH}/UNITTESTS/fakes/events/EventQueue.cpp
)

target_include_directories(mbed-os-event-queue-fake
    PUBLIC
        ${MBED_PATH}/UNITTESTS/fakes
)

target_link_libraries(mbed-os-event-queue-fake
    PRIVATE
        mbed-os-stubs-headers
)
