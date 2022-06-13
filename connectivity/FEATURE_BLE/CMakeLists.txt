# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(NOT MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/UNITTESTS)
    endif()
endif()

add_library(mbed-ble STATIC EXCLUDE_FROM_ALL)
add_library(mbed-ble-cordio_ll STATIC EXCLUDE_FROM_ALL)

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
