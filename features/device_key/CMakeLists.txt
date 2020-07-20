# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
	PUBLIC
		source
)

target_sources(mbed-os
	PRIVATE
		source/DeviceKey.cpp
)
