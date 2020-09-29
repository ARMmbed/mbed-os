# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-cellular-alt1250 INTERFACE)

target_include_directories(mbed-os-cellular-alt1250
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-alt1250
    INTERFACE
        ALT1250_PPP.cpp
        ALT1250_PPP_CellularContext.cpp
        ALT1250_PPP_CellularNetwork.cpp
)

target_link_libraries(mbed-os-cellular-alt1250 INTERFACE mbed-os-cellular)
