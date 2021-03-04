# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-802.15.4-rf INTERFACE)

add_library(mbed-lorawan INTERFACE)
add_library(mbed-nfc INTERFACE)
add_library(mbed-ppp INTERFACE)
add_library(mbed-wifi INTERFACE)

add_subdirectory(FEATURE_BLE)
add_subdirectory(cellular)
add_subdirectory(libraries)
add_subdirectory(lorawan)
add_subdirectory(lwipstack)
add_subdirectory(mbedtls)
add_subdirectory(nanostack)
add_subdirectory(netsocket)
add_subdirectory(nfc)

# Drivers to be the last due to dependencies between stacks and drivers
add_subdirectory(drivers)
