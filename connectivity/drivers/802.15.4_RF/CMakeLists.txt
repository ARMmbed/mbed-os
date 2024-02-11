# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# The 'mbed-802.15.4-rf' target is lazily created because there might not be any subdirectories needing it at all.
macro(create_mbed_802_15_4_target)
	if(NOT TARGET mbed-802.15.4-rf)
		add_library(mbed-802.15.4-rf STATIC EXCLUDE_FROM_ALL)

		# Nanostack drivers always require Mbed RTOS
		target_link_libraries(mbed-802.15.4-rf PUBLIC mbed-core-flags mbed-rtos-flags)

		target_link_libraries(mbed-nanostack
		    INTERFACE
		        mbed-802.15.4-rf
		)
	endif()
endmacro(create_mbed_802_15_4_target)


if("Freescale" IN_LIST MBED_TARGET_LABELS)
	create_mbed_802_15_4_target()
    add_subdirectory(TARGET_Freescale)
elseif("Silicon_Labs" IN_LIST MBED_TARGET_LABELS)
	create_mbed_802_15_4_target()
    add_subdirectory(TARGET_Silicon_Labs)
endif()

if("COMPONENT_MICROCHIP_AT86RF=1" IN_LIST MBED_TARGET_DEFINITIONS)
	create_mbed_802_15_4_target()
	add_subdirectory(COMPONENT_MICROCHIP_AT86RF)
endif()

if("COMPONENT_NXP_MCR20A=1" IN_LIST MBED_TARGET_DEFINITIONS)
	create_mbed_802_15_4_target()
	add_subdirectory(COMPONENT_NXP_MCR20A)
endif()

if("COMPONENT_STM_S2_LP=1" IN_LIST MBED_TARGET_DEFINITIONS)
	create_mbed_802_15_4_target()
	add_subdirectory(COMPONENT_STM_S2_LP)
endif()


