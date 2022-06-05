# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# These five directories create targets that are then used by buildfiles under drivers/
add_subdirectory(nanostack)
add_subdirectory(cellular)
add_subdirectory(mbedtls)
add_subdirectory(nfc)
if("FEATURE_BLE=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(FEATURE_BLE)
endif()

add_subdirectory(drivers)
add_subdirectory(libraries)
add_subdirectory(lorawan)
add_subdirectory(lwipstack)
add_subdirectory(netsocket)