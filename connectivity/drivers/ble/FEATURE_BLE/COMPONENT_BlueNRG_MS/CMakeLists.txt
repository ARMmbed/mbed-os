# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-bluenrg-ms STATIC EXCLUDE_FROM_ALL
	BlueNrgMsHCIDriver.cpp)

target_link_libraries(mbed-bluenrg-ms
    PUBLIC
        mbed-core-flags
        mbed-ble
    PRIVATE
        mbed-rtos-flags)


# circular dependency between mbed-ble and the BlueNRG driver, because of the ble_cordio_get_hci_driver() implementation
target_link_libraries(mbed-ble PUBLIC mbed-bluenrg-ms)