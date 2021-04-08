# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fakes-ble)

target_include_directories(mbed-fakes-ble
    PUBLIC
        ${MBED_PATH}/UNITTESTS/fakes/ble/
        ${MBED_PATH}/connectivity/FEATURE_BLE/include
        ${MBED_PATH}/connectivity/FEATURE_BLE/include/ble
        ${MBED_PATH}/connectivity/FEATURE_BLE/source
    PRIVATE
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
)

target_sources(mbed-fakes-ble
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
        ${MBED_PATH}/UNITTESTS/fakes/ble/source/GattServerImpl_mock.cpp
        ${MBED_PATH}/UNITTESTS/fakes/ble/ble_mocks.h
        ${MBED_PATH}/UNITTESTS/fakes/ble/GapImpl_mock.h
        ${MBED_PATH}/UNITTESTS/fakes/ble/GattClientImpl_mock.h
        ${MBED_PATH}/UNITTESTS/fakes/ble/GattServerImpl_mock.h
        ${MBED_PATH}/UNITTESTS/fakes/ble/SecurityManagerImpl_mock.h
)

target_link_libraries(mbed-fakes-ble
    PRIVATE
        mbed-headers
        mbed-stubs-headers
        gcov
)
