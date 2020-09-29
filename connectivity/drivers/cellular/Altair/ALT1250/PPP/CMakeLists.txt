# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-alt1250 OBJECT)

target_include_directories(mbed-os-cellular-alt1250
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-alt1250
    PRIVATE
        ALT1250_PPP.cpp
        ALT1250_PPP_CellularContext.cpp
        ALT1250_PPP_CellularNetwork.cpp
)

mbed_link_component(mbed-os-cellular-alt1250 mbed-os-cellular mbed-os)
