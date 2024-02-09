# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Note that the RM1000 chip appears to be unbuyable and the RIOT_MICRO_MODULE target that used it was
# already removed from Mbed in Mbed 6.0.0.  This would make it a candidate for removal.
if("COMPONENT_RIOTMICRO_RM1000=1" IN_LIST MBED_TARGET_DEFINITIONS)
	add_subdirectory(COMPONENT_RIOTMICRO_RM1000)
endif()