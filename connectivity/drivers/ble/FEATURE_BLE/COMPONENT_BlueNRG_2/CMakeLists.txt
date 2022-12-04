# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-bluenrg-2 STATIC EXCLUDE_FROM_ALL
	BlueNrg2HCIDriver.cpp)

target_link_libraries(mbed-bluenrg-2 PUBLIC
	mbed-os
	mbed-ble)
