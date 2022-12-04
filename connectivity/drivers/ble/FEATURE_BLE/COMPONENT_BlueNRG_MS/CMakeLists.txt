# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-bluenrg-ms STATIC EXCLUDE_FROM_ALL
	BlueNrgMsHCIDriver.cpp)

target_link_libraries(mbed-bluenrg-ms PUBLIC
	mbed-os
	mbed-ble)