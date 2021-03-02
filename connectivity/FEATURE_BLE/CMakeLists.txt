# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble OBJECT)
# cordio libraries are part of ble stack (_ll is optional)
# We use INTERFACE libraries to form the main object library otherwise we would 
# get circular deps error between ble <-> cordio
add_library(mbed-ble-cordio INTERFACE)
add_library(mbed-ble-cordio_ll INTERFACE)

# Add drivers for BLE
# Once we remove the old tools and their path requirements, we should move 
# drivers to BLE stack to be in the same place
add_subdirectory(../drivers/ble ${CMAKE_CURRENT_BINARY_DIR}/drivers/ble)
add_subdirectory(libraries)
add_subdirectory(source)

target_include_directories(mbed-ble
    PUBLIC
        .
        include
        include/ble
        include/ble/common
        include/ble/common/ble
        include/ble/common/ble/gap
        include/ble/compatibility
        include/ble/compatibility/ble
        include/ble/driver
        include/ble/gap
        include/ble/gatt
        include/ble/services
        source
)

target_compile_definitions(mbed-ble
    PUBLIC
        MBED_CONF_BLE_PRESENT=1
)

target_link_libraries(mbed-ble PRIVATE mbed-ble-cordio mbed-core ${MBED_BLE_TARGET_LIBRARIES})
