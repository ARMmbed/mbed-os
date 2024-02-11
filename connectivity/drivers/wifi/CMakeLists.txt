# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# The 'mbed-wifi' target is lazily created because there might not be any subdirectories needing it at all.
macro(create_mbed_wifi_target)
	if(NOT TARGET mbed-wifi)
		add_library(mbed-wifi STATIC EXCLUDE_FROM_ALL)

		target_link_libraries(mbed-wifi
		    PUBLIC
		        mbed-rtos-flags
		        mbed-netsocket-api
		)
	endif()
endmacro()


# The WICED subdirectory is for wifi drivers developed using Infineon WICED framework.
# https://community.infineon.com/t5/Knowledge-Base-Articles/WICED-Wi-Fi-FAQ/ta-p/247356
if("WICED" IN_LIST MBED_TARGET_LABELS)
	create_mbed_wifi_target()
    add_subdirectory(TARGET_WICED EXCLUDE_FROM_ALL)
endif()

if("STM" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM EXCLUDE_FROM_ALL)
endif()

if("WHD" IN_LIST MBED_TARGET_LABELS)
	create_mbed_wifi_target()
    add_subdirectory(COMPONENT_WHD EXCLUDE_FROM_ALL)
endif()

if("COMPONENT_ESPRESSIF_ESP8266=1" IN_LIST MBED_TARGET_DEFINITIONS)
	create_mbed_wifi_target()
    add_subdirectory(COMPONENT_ESPRESSIF_ESP8266)
endif()

