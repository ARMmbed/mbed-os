# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble OBJECT)

add_subdirectory(libraries)
add_subdirectory(source)

target_include_directories(mbed-os-ble
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

target_compile_definitions(mbed-os-ble
    PUBLIC
        MBED_CONF_BLE_PRESENT=1
)

mbed_link_component(mbed-os-ble mbed-os)
