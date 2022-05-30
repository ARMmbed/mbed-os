# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# List of all connectivity libraries available.
add_library(mbed-802.15.4-rf INTERFACE)
add_library(mbed-ble INTERFACE)
add_library(mbed-ble-cordio INTERFACE)
add_library(mbed-ble-cordio_ll INTERFACE)
add_library(mbed-emac INTERFACE)
add_library(mbed-lorawan INTERFACE)
add_library(mbed-mbedtls STATIC EXCLUDE_FROM_ALL)

add_library(mbed-nfc INTERFACE)
add_library(mbed-ppp INTERFACE)
add_library(mbed-wifi INTERFACE)

add_subdirectory(nanostack)
add_subdirectory(cellular)
add_subdirectory(drivers)
add_subdirectory(FEATURE_BLE)
add_subdirectory(libraries)
add_subdirectory(lorawan)
add_subdirectory(lwipstack)
add_subdirectory(mbedtls)
add_subdirectory(netsocket)
add_subdirectory(nfc)