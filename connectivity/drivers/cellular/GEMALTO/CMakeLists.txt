# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("COMPONENT_GEMALTO_CINTERION=1" IN_LIST MBED_TARGET_DEFINITIONS)
	add_subdirectory(CINTERION)
endif()