# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(cordio)
add_subdirectory(gap)
add_subdirectory(gatt)
add_subdirectory(generic)
add_subdirectory(pal)

target_sources(mbed-ble
    INTERFACE
        BLE.cpp
        Gap.cpp
        GattClient.cpp
        GattServer.cpp
        SecurityManager.cpp
)

target_link_libraries(mbed-ble
    INTERFACE
        mbed-ble-cordio
)
