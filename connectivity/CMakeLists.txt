# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-802.15.4-rf INTERFACE)
add_library(mbed-ble INTERFACE)
add_library(mbed-ble-blue_nrg INTERFACE)
add_library(mbed-ble-cordio INTERFACE)
add_library(mbed-ble-cordio_ll INTERFACE)
add_library(mbed-lorawan INTERFACE)
add_library(mbed-nfc INTERFACE)
add_library(mbed-wifi INTERFACE)
add_subdirectory(FEATURE_BLE EXCLUDE_FROM_ALL)
add_subdirectory(cellular EXCLUDE_FROM_ALL)
add_subdirectory(libraries EXCLUDE_FROM_ALL)
add_subdirectory(lorawan EXCLUDE_FROM_ALL)
add_subdirectory(lwipstack EXCLUDE_FROM_ALL)
add_subdirectory(mbedtls EXCLUDE_FROM_ALL)
add_subdirectory(nanostack EXCLUDE_FROM_ALL)
add_subdirectory(netsocket EXCLUDE_FROM_ALL)
add_subdirectory(nfc EXCLUDE_FROM_ALL)

# Drivers to be the last due to dependencies between stacks and drivers
add_subdirectory(drivers)
