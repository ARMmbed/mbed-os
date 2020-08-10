# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/internal
)

target_sources(mbed-os
    PRIVATE
        source/GapImpl.cpp
        source/GattClientImpl.cpp
        source/GattServerImpl.cpp
        source/SecurityManagerImpl.cpp
)
