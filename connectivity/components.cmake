# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

get_property(_internal_components GLOBAL PROPERTY mbed-os-internal-components)

list(APPEND connectivity_components
    mbed-os-cellular
    mbed-os-ble
    mbed-os-lorawan
    mbed-os-mbedtls
    mbed-os-nanostack
    mbed-os-netsocket
    mbed-os-nfc
)

# Only add once
foreach(component IN LISTS connectivity_components)
    if(NOT component IN_LIST _internal_components)
        list(APPEND _internal_components ${component})
    endif()
endforeach()

set_property(GLOBAL PROPERTY mbed-os-internal-components ${_internal_components})

# Components paths to the main CMake file
set(mbed-os-ble_PATH connectivity/FEATURE_BLE)
set(mbed-os-nfc_PATH connectivity/nfc)
set(mbed-os-cellular_PATH connectivity/cellular)
set(mbed-os-lorawan_PATH connectivity/lorawan)
set(mbed-os-mbedtls_PATH connectivity/mbedtls)
set(mbed-os-nanostack_PATH connectivity/nanostack)
set(mbed-os-netsocket_PATH connectivity/netsocket)
