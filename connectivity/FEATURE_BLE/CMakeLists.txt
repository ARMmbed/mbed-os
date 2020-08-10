# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(libraries)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/common/ble
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/common/ble/gap
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/services
)

target_sources(mbed-os
    PRIVATE
        source/BLE.cpp
        source/DiscoveredCharacteristic.cpp
        source/FileSecurityDb.cpp
        source/KVStoreSecurityDb.cpp

        source/gap/AdvertisingDataBuilder.cpp
        source/gap/AdvertisingParameters.cpp
        source/gap/ConnectionParameters.cpp
)
